// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_Melee.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URPGGA_Monster_Attack_Melee::URPGGA_Monster_Attack_Melee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AttackMontages.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, AttackMontages.Num() -1);

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
this,TEXT("Player_Light_Attack"), AttackMontages[RandomIndex], 1.0f,  NAME_None,
true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_Melee::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_Melee::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_Melee::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_Melee::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();
	}
}

void URPGGA_Monster_Attack_Melee::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
