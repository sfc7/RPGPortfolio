// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"

#include "GameAbilitySystem/RPGAttributeSet.h"


void UPlayerAttackSkillGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerAttackSkillGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FString UPlayerAttackSkillGameplayAbility::GetSkillDescriptionForUI()
{
	int32 CurrentLevel = GetAbilityLevel();

	// Level로 부터 현재 어빌리티 정보 계산
	const float ScaledDamage = DamageScale.GetValueAtLevel(CurrentLevel);
	const float CostMana = FMath::Abs(GetCostMana(CurrentLevel));
	const float Cooldown = GetCooldown(CurrentLevel);

	// String 형태로 제작
	return *FString::Printf(TEXT("데미지 x%.1f 소모마나 %.1f 쿨다운 %.1f"), ScaledDamage, CostMana, Cooldown);;
}