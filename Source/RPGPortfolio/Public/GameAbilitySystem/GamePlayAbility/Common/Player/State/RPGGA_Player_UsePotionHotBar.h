// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_UsePotionHotBar.generated.h"

/**
 * UURPGGA_Player_UsePotionHotBar
 * 
 * 플레이어용 퀵슬롯 아이템 사용 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_UsePotionHotBar : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_UsePotionHotBar();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//@ 포션 사용 데이터 처리할 GameplayEffect 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> PotionHealEffectClass;

	//@ 포션 사용 이펙트용 FGameplayTag 큐
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag PotionUseFXGameplayCue;

	//@ 힐 이펙트 적용
	UFUNCTION()
	void ApplyHealEffect(int32 HealAmount);
};

