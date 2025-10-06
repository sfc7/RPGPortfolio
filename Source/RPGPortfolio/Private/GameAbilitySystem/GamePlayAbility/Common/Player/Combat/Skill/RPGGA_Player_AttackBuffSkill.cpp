// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_AttackBuffSkill.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Player/PlayerSkillComponent.h"

URPGGA_Player_AttackBuffSkill::URPGGA_Player_AttackBuffSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_AttackBuffSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//@ 마나 소모 및 쿨다운 적용
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent())
	{
		// 퀵슬롯에 해당 스킬이 등록되어 있다면 쿨다운 UI 표시
		int32 QuickSlotIndex = GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent()->FindQuickSlotIndexByTag(RPGGameplayTag::Player_Ability_Skill_AttackBuff);
        
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast(
			QuickSlotIndex, 
			RPGGameplayTag::Player_Ability_Skill_AttackBuff, 
			GetCooldownTimeRemaining(), 
			GetCooldownTimeRemaining()
		);
	}

	// 버프 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
	TEXT("Player_AttackBuff_Skill"), BuffCastingMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 버프 데미지 계산, 이펙트 등의 로직을 바인딩
	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this,RPGGameplayTag::Player_Event_ActiveBuff,nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &URPGGA_Player_AttackBuffSkill::ApplyBuffSkill);
	GameplayEventTask->ReadyForActivation();
}

void URPGGA_Player_AttackBuffSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_AttackBuffSkill::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_AttackBuffSkill::ApplyBuffSkill(FGameplayEventData PayloadData)
{
	// 버프스킬의 GameplayEffect를 생성후 기간도 설정하고 ASC에 등록
	FGameplayEffectContextHandle EffectContextHandle = GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->MakeOutgoingSpec(BuffEffectClass, 1, EffectContextHandle);
	if (SpecHandle.IsValid())
	{
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
		Spec->SetDuration(BuffDuration.GetValue(), true);

		GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
	}

	// 버프 실행 시 이펙트 GameplayCue 실행
	FGameplayCueParameters GCEffectParam;
	GCEffectParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->AddGameplayCue(BuffFXGameplayCue, GCEffectParam);

	// 버프 지속시간 이후 제거 관련 실행
	FTimerHandle GCRemoveTimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(GCRemoveTimerHandle, [this]()
	{
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->RemoveGameplayCue(BuffFXGameplayCue);
	}, BuffDuration.GetValue(), false);
}
	