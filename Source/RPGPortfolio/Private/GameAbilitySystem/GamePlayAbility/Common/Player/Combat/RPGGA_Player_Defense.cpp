// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_Defense.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintGameplayTagLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"

URPGGA_Player_Defense::URPGGA_Player_Defense()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_Defense::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
	TEXT("Player_Defense"), DefenseMontage, 1.0f, NAME_None,
	true, 1.0f, false);
	
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Defense::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Defense::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	FGameplayCueParameters DefenseGCEffectCueParam;
	DefenseGCEffectCueParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->AddGameplayCue(DefenseEffectGamePlayCue, DefenseGCEffectCueParam);

	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, RPGGameplayTag::Player_Event_DefenseSuccess, nullptr, false, true
	);
	WaitGameplayEvent->EventReceived.AddDynamic(this, &URPGGA_Player_Defense::SuccessDefenseCallback);
	WaitGameplayEvent->ReadyForActivation();
}

void URPGGA_Player_Defense::SuccessDefenseCallback(FGameplayEventData PayloadData)
{
	FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector PlayerBackVector = GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * -1;
	FVector TargetLocation = PayloadData.Instigator->GetActorLocation();

	GetPlayerCharacterFromActorInfo()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation));
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

	FGameplayCueParameters DefenseSucessGCCueParam;
	DefenseSucessGCCueParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DefenseSuccessGamePlayCue, DefenseSucessGCCueParam);
}

void URPGGA_Player_Defense::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
