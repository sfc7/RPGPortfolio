// LJS


#include "GameAbilitySystem/GamePlayAbility/Player/Combat/RPGGA_Player_Attack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URPGGA_Player_Attack::URPGGA_Player_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboCount = 1;
}

void URPGGA_Player_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	UAnimMontage* MontageToPlay = nullptr;
	UE_LOG(LogTemp, Log, TEXT("%d"), CurrentComboCount);
	
	if(LightAttackMontages.Contains(CurrentComboCount))
	{
		MontageToPlay = LightAttackMontages[CurrentComboCount];
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Player_Light_Attack"),
		MontageToPlay, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Attack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Attack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Attack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Attack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();
	
	if (CurrentComboCount == LightAttackMontages.Num())
	{
		ResetComboCount();
	}
	else
	{
		CurrentComboCount++;
	}
}

void URPGGA_Player_Attack::ResetComboCount()
{
	CurrentComboCount = 1;
}

void URPGGA_Player_Attack::ResetComboCountTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &URPGGA_Player_Attack::ResetComboCount, 0.3f, false);
}

void URPGGA_Player_Attack::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	ResetComboCountTimer();
}
