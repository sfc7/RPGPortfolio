// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_Defense.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintGameplayTagLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URPGGA_Player_Defense::URPGGA_Player_Defense()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_Defense::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DefenseActivateTime = GetWorld()->GetTimeSeconds();
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
	TEXT("Player_Defense"), DefenseMontage, 1.0f, NAME_None,
	true, 1.0f, false);
	
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Defense::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Defense::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	FGameplayCueParameters DefenseGCEffectParam;
	DefenseGCEffectParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->AddGameplayCue(DefenseEffectGamePlayCue, DefenseGCEffectParam);

	UAbilityTask_WaitGameplayEvent* DefenseSuccessGE = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, RPGGameplayTag::Player_Event_DefenseSuccess, nullptr, false, true
	);
	DefenseSuccessGE->EventReceived.AddDynamic(this, &URPGGA_Player_Defense::SuccessDefenseCallback);
	DefenseSuccessGE->ReadyForActivation();
}

void URPGGA_Player_Defense::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) != 1.0f)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}

	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->RemoveGameplayCue(DefenseEffectGamePlayCue);
}

void URPGGA_Player_Defense::SuccessDefenseCallback(FGameplayEventData PayloadData)
{
	FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector PlayerBackVector = GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * -1;
	FVector TargetLocation = PayloadData.Instigator->GetActorLocation();

	// GetPlayerCharacterFromActorInfo()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation));
	UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce( this,                         
		TEXT("None"),      
		PlayerBackVector,         
		150.0f,                        
		0.2f,                      
		false,                      
		nullptr,                    
		ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
		FVector::ZeroVector,         
		0.0f, false);
	
	bool bIsParrying = (GetWorld()->GetTimeSeconds() - DefenseActivateTime) <= 0.5f;
	
	if(bIsParrying)
	{
		SetParryingAttackReady();
		
		FGameplayCueParameters DefenseParryingGCParam;
		DefenseParryingGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
		GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DefenseParryingGamePlayCue, DefenseParryingGCParam);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

		RemoveParryingAttackReady();
		GetWorld()->GetTimerManager().SetTimer(ParryingInputTimerHandle, [this]()
		{
			RemoveGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanParryingAttack);
		}, 0.5f, false);
	}
	else
	{
		FGameplayCueParameters DefenseSucessGCParam;
		DefenseSucessGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
		GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DefenseSuccessGamePlayCue, DefenseSucessGCParam);
	}
	

}

void URPGGA_Player_Defense::SetParryingAttackReady()
{
	// BP_ApplyGameplayEffectToOwner(InvincibleEffectClass, GetAbilityLevel(), 1); 
	
	AddGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanParryingAttack); // IMC 관련
	
	FGameplayAbilitySpec ParryAbilitySpec(ParryingAttackGA);
	ParryAbilitySpec.SourceObject = GetPlayerCharacterFromActorInfo();
	ParryAbilitySpec.Level = GetAbilityLevel();
	ParryAbilitySpec.GetDynamicSpecSourceTags().AddTag(ParryingInputTag);
	ParryAbilityHandle = GetRPGAbilitySystemComponentFromActorInfo()->GiveAbility(ParryAbilitySpec);
}

void URPGGA_Player_Defense::RemoveParryingAttackReady()
{
	GetWorld()->GetTimerManager().SetTimer(ParryingDelayAndGATimerHandle, [this]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		
		if (ParryAbilityHandle.IsValid())
		{
			FGameplayAbilitySpec* Spec = GetRPGAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(ParryAbilityHandle);
			if (Spec && Spec->IsActive())
			{
				GetWorld()->GetTimerManager().SetTimer(ParryingDelayAndGATimerHandle, [this]()
				{
					GetRPGAbilitySystemComponentFromActorInfo()->ClearAbility(ParryAbilityHandle);
					ParryAbilityHandle = FGameplayAbilitySpecHandle(); 
				}, 1.0f, false);
			}
		}
	}, 0.2f, false);
}

void URPGGA_Player_Defense::OnEndAbilityCallback()
{
	RemoveParryingAttackReady();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
