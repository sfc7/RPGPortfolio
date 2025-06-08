// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_Range.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URPGGA_Monster_Attack_Range::URPGGA_Monster_Attack_Range()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_Range::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AttackMontage)
	{

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Attack_Range"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();
	}
}

void URPGGA_Monster_Attack_Range::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
