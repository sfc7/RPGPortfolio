// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_Defense.generated.h"

class UPlayerGameplayAbility;
class UGameplayEffect;
class UCombatManager;
/**
 * URPGGA_Player_Defense
 * 
 * 플레이어 디펜스 어빌리티 입력 시 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Defense : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Defense();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 방어 성공시 콜백 함수
	UFUNCTION()
	void SuccessDefenseCallback(FGameplayEventData PayloadData);

	//@ 방어 애니메이션 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DefenseMontage;

	// 방어시 이펙트 보여줄 GameplayCue의 GameplayTag (GameplayTag로 Gameplay Cue를 실행함)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseEffectGamePlayCue;

	// 방어 성공 시 이펙트 보여줄 GameplayCue의 GameplayTag (GameplayTag로 Gameplay Cue를 실행함)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseSuccessGamePlayCue;

	float DefenseActivateTime;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();
	
	// 패링시 이펙트 보여줄 GameplayCue의 GameplayTag (GameplayTag로 Gameplay Cue를 실행함)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DefenseParryingGamePlayCue;

	// 패링 어택의 GameplayAbility
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UPlayerGameplayAbility> ParryingAttackGA;

	// 패링 어택 Input 받을 GameplayTag 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ParryingInputTag;

	// 무적 관련 GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> InvincibleEffectClass;

	//@ 패링 어택 지연 및 GA 타이머 핸들
	FTimerHandle ParryingDelayAndGATimerHandle;
	//@ 패링 어택 입력 타이머 핸들
	FTimerHandle ParryingInputTimerHandle;
	//@ 페이드백 타이머 핸들
	FTimerHandle FadeBackTimerHandle;

	//@ 패링 어택 어빌리티 핸들
	FGameplayAbilitySpecHandle ParryAbilityHandle;

	//@ 패링 어택 준비 설정
	UFUNCTION()
	void SetParryingAttackReady();

	//@ 패링 어택 준비 제거
	UFUNCTION()
	void RemoveParryingAttackReady();

	//@ 캐시된 전투 매니저
	UCombatManager* CachedCombatManager;

	//@ 패링 성공 여부 체크를 위한 시간
	float ParryingSuccessCheckTime = 1.0f;
	//@ 패링 어택 가능 상태 시간
	float CanParryingAttackStateDelayTime = 0.5f;
	//@ 패링 어택이 ASC에 활성화 되어 있는 시간 
	float ParryingAttackASCActiveTime = 1.0f;
	float GlobalTimeDilation = 0.2f;
};
