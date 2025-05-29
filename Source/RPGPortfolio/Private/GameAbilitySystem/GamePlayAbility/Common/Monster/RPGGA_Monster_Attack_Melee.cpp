// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_Melee.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

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

		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, RPGGameplayTag::Character_Event_AttackHit_Melee, nullptr, false, true
		);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::ApplyDamage);
		WaitGameplayEvent->ReadyForActivation();

		
	}
}

void URPGGA_Monster_Attack_Melee::OnEventReceived(FGameplayEventData PayloadData)
{
	UE_LOG(LogTemp, Log, TEXT("Instigator : %s, Target : %s"), *PayloadData.Instigator.Get()->GetName(), *PayloadData.Target.Get()->GetName());
}

void URPGGA_Monster_Attack_Melee::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Monster_Attack_Melee::ApplyDamage(FGameplayEventData PayloadData)
{
	LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());
	FGameplayEffectSpecHandle SpecHandle = MakeMonsterDamageEffectSpecHandle(DamageEffectClass, DamageScale);
	ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
}

//ApplyEffectsSpecHandleToTargetCallback 비교 필요
