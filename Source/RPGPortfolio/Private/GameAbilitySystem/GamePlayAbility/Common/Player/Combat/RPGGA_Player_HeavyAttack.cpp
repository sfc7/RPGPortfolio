// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_HeavyAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/PlayerCharacterBase.h"


URPGGA_Player_HeavyAttack::URPGGA_Player_HeavyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_HeavyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLightAttackComboCount = 1;
	
	if (HasMatchingGameplayTag(RPGGameplayTag::Player_Status_CanHeavyAttackCombo))
	{
		CurrentLightAttackComboCount = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute());
	}

	UAnimMontage* MontageToPlay = nullptr;
	if(HeavyAttackMontages.Contains(CurrentLightAttackComboCount))
	{
		MontageToPlay = HeavyAttackMontages[CurrentLightAttackComboCount];	
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_Heavy_Attack"), MontageToPlay, 1.0f,  NAME_None,
	true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_HeavyAttack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_HeavyAttack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_HeavyAttack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_HeavyAttack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,RPGGameplayTag::Character_Event_AttackHit_Melee,nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::ApplyEffectsSpecHandleToTargetCallback);
	GameplayEventTask->ReadyForActivation();
}

void URPGGA_Player_HeavyAttack::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_HeavyAttack::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());
	float WeaponDamage = PayloadData.EventMagnitude;
	FGameplayEffectSpecHandle SpecHandle = MakePlayerDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, RPGGameplayTag::Data_DamageType_SetByCaller_Heavy, CurrentLightAttackComboCount);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}
}
