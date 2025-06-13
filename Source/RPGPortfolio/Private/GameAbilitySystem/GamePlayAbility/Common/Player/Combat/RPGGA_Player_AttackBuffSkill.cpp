// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_AttackBuffSkill.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

URPGGA_Player_AttackBuffSkill::URPGGA_Player_AttackBuffSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_AttackBuffSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
	TEXT("Player_AttackBuff_Skill"), BuffCastingMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this,RPGGameplayTag::Player_Event_ActiveBuff,nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::ApplyFXGameplayCue);
	GameplayEventTask->ReadyForActivation();
}

void URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_AttackBuffSkill::ApplyFXGameplayCue(FGameplayEventData PayloadData)
{
	FGameplayCueParameters GCEffectParam;
	GCEffectParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->AddGameplayCue(BuffFXGameplayCue, GCEffectParam);
}
