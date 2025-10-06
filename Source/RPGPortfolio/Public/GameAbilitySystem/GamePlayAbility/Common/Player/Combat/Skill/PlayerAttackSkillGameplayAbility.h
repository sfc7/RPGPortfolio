// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "PlayerAttackSkillGameplayAbility.generated.h"

/**
 * UPlayerAttackSkillGameplayAbility
 * 
 * 플레이어 스킬 어빌리티의 기본 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerAttackSkillGameplayAbility : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
	
public:
	//@ UI용 스킬 어빌리티의 설명 텍스트 생성
	FString GetSkillDescriptionForUI();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 스킬 데미지 배율 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale; 
};
