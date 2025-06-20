// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_LightAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerAttributeSet.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"

URPGGA_Player_LightAttack::URPGGA_Player_LightAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	bool bRotate = FindNearestEnemyBeforeAttack(1000.f); 

	if (bRotate)
	{
		URPGAT_Player_RotateTarget* RotateTickTask = URPGAT_Player_RotateTarget::ExecuteTaskOnTick(this);
		RotateTickTask->OnRotateTargetTaskTick.AddDynamic(this, &UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack);
		RotateTickTask->SetTargetRotation(FindRototation);
		RotateTickTask->OnRotationCompleted.AddDynamic(this, &URPGGA_Player_LightAttack::Attack);
		RotateTickTask->ReadyForActivation();
	}
	else
	{	
		Attack();
	}
}

void URPGGA_Player_LightAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_LightAttack::Attack()
{
	UPlayerCombatComponent* CombatComponent = GetPlayerCombatComponentFromActorInfo();
	if (CombatComponent)
	{
		AWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCurrentEquippedWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->SetCurrentAttackType(EWeaponAttackType::Light);
		}
	}
	
	float CurrentComboCount  = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute());
	UsedComboCount = CurrentComboCount;
	
	UAnimMontage* MontageToPlay = nullptr;
	if(LightAttackMontages.Contains(CurrentComboCount))
	{
		MontageToPlay = LightAttackMontages[CurrentComboCount];
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,TEXT("Player_Light_Attack"), MontageToPlay, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,RPGGameplayTag::Character_Event_AttackHit_Melee,nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::ApplyEffectsSpecHandleToTargetCallback);
	GameplayEventTask->ReadyForActivation();

	
	if (CurrentComboCount == LightAttackMontages.Num())
	{
		ResetComboCount();
	}
	else
	{
		AddGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanHeavyAttackCombo);
		GetAbilitySystemComponentFromActorInfo()->SetNumericAttributeBase(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute(), CurrentComboCount + 1);
	}
}

void URPGGA_Player_LightAttack::ResetComboCount()
{
	GetAbilitySystemComponentFromActorInfo()->SetNumericAttributeBase(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute(), 1);
	RemoveGameplayTag(GetPlayerCharacterFromActorInfo(), RPGGameplayTag::Player_Status_CanHeavyAttackCombo);
}

void URPGGA_Player_LightAttack::ResetComboCountTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &URPGGA_Player_LightAttack::ResetComboCount, 0.5f, false);
}

void URPGGA_Player_LightAttack::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	ResetComboCountTimer();
}

void URPGGA_Player_LightAttack::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());
	float WeaponDamage = PayloadData.EventMagnitude;
	FGameplayEffectSpecHandle SpecHandle = MakePlayerComboDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, RPGGameplayTag::Data_DamageType_SetByCaller_Light, UsedComboCount);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		ASC->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Light, EffectContext);
	}
	
	FActiveGameplayEffectHandle HitReactGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (HitReactGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}

	BP_ApplyGameplayEffectToOwner(GainMpEffectClass,GetAbilityLevel());
}
