// LJS


#include "GameAbilitySystem/GamePlayAbility/Player/Combat/RPGGA_Player_LightAttack.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Player/PlayerAttributeSet.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URPGGA_Player_LightAttack::URPGGA_Player_LightAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	float CurrentComboCount  = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UPlayerAttributeSet::GetCurrentLightAttackComboAttribute());
	
	UE_LOG(LogTemp,Log,TEXT("%f"), CurrentComboCount);
	
	UAnimMontage* MontageToPlay = nullptr;
	if(LightAttackMontages.Contains(CurrentComboCount))
	{
		MontageToPlay = LightAttackMontages[CurrentComboCount];
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Player_Light_Attack"),
		MontageToPlay, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_LightAttack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();
	
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
