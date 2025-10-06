// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "PlayerCombatGameplayAbility.generated.h"

class URPGAT_Player_RotateTarget;
/**
 * UPlayerCombatGameplayAbility
 * 
 * 플레이어 전투 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCombatGameplayAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayerCombatGameplayAbility();

	//@ 공격 전 타겟 회전 업데이트
	UFUNCTION(BlueprintCallable)
	void RotateTargetTickBeforeAttack(float DeltaTime);
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 데미지 계산용 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	//@ 공격 전 가장 가까운 적 탐지
	UFUNCTION()
	bool FindNearestEnemyBeforeAttack(float BoxExtent);

	//@ 회전 대상 액터 캐싱
	UPROPERTY()
	AActor* RotateTargetActor;

	//@ 찾은 회전 값 캐싱
	UPROPERTY()
	FRotator FindRotation;
};
