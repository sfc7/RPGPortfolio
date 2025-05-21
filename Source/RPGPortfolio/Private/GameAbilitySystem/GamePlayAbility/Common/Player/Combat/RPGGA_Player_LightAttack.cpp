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

URPGGA_Player_LightAttack::URPGGA_Player_LightAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

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
	FGameplayEffectSpecHandle SpecHandle = MakePlayerDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, RPGGameplayTag::Data_DamageType_SetByCaller_Light, UsedComboCount);
	
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}
}
