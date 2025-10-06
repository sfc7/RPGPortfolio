// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_AttackBuffSkill.generated.h"

/**
 * URPGGA_Player_AttackBuffSkill
 * 
 * 플레이어 버프 스킬 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_AttackBuffSkill : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_AttackBuffSkill();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ GameplayEventData로 버프 적용 로직
	UFUNCTION()
	void ApplyBuffSkill(FGameplayEventData PayloadData);

	//@ 버프 적용 시 지속적으로 보여줄 FX의 GameplayTag
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag BuffFXGameplayCue;

	//@ 버프 적용 시 사용할 GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> BuffEffectClass;

	//@ 버프 기간 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buff")
	FScalableFloat BuffDuration;

	//@ 버프 실행 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* BuffCastingMontage;
};
