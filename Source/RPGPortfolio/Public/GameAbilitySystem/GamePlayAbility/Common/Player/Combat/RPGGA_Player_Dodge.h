// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_Dodge.generated.h"

/**
 * URPGGA_Player_Dodge
 * 
 * 플레이어 회피 어빌리티 입력 시 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Dodge : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Dodge();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 회피 방향 및 거리 계산
	UFUNCTION()
	void CalcDodgeDirectionAndDistance();

	//@ 회피 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DodgeMontage;

	//@ 회피 Montage 방향 모션 워핑용 FName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDirection;

	//@ 회피 Montage 거리 모션 워핑용 FName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WarpTargetNameDistance;

	//@ 회피 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat Distance;

	//@ 회피 방향
	FVector DodgeDirection;

	//@ 이펙트 및 연출 용 회피 성공 콜백 함수
	UFUNCTION()
	void SuccessDodgeCallback(FGameplayEventData PayloadData);

	//@ 회피 이펙트 GameplayCue
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DodgeEffectGamePlayCue;

	//@ 회피 성공 이펙트 GameplayCue
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Categories=GameplayCue))
	FGameplayTag DodgeSuccessGamePlayCue;

	//@ 회피 성공 이펙트용 타이머 핸들
	FTimerHandle DodgeDelayTimerHandle;

	//@ 페이드백 타이머 핸들
	FTimerHandle FadeBackTimerHandle;


	
};
