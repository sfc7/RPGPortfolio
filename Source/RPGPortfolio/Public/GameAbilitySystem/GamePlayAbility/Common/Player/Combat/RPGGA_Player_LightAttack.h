// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_LightAttack.generated.h"

class UAnimMontage;
class UNiagaraSystem;
/**
 * URPGGA_Player_LightAttack
 * 
 * 플레이어 약 공격 어빌리티 입력 시 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_LightAttack : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_LightAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 공격 실행 로직
	UFUNCTION()
	void Attack();

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 타겟에게 GameplayEffect 적용(데미지, 이펙트 등)
	UFUNCTION()
	void ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData);

	//@ 콤보 카운트 리셋
	UFUNCTION()
	void ResetComboCount();

	//@ 콤보 카운트 리셋 타이머 실행 함수
	UFUNCTION()
	void ResetComboCountTimer();

	//@ 콤보 카운트 리셋 타이머
	FTimerHandle ComboTimerHandle;

	// 실제 콤보 카운트
	float UsedComboCount;

	//@ 타겟 액터
	AActor* LocalTargetActor;

	//@ 약 공격 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<int32, UAnimMontage*> LightAttackMontages;

	//@ 약 공격 성공 시 마나 획득 GamepalyEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GainMpEffectClass;
	
};
