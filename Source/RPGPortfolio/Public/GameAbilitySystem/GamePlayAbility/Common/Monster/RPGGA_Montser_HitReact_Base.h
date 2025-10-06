// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Montser_HitReact_Base.generated.h"

/**
 * URPGGA_Montser_HitReact_Base
 * 
 * 몬스터의 피격을 처리하는 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Montser_HitReact_Base : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	//@ 피격 Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> HitReactMontages;

	//@ 피격 Montage를 사용할건지 체크 플래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasHitReactMontagesToPlay = true;

	//@ 몬스터가 공격 받는 중인지 GameplayTag를 부여할 GameplayEffect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> IsBeingAttackedGameplayEffect;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 피격시 상대방을 보는 방향으로 할건지 플래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseFaceAttacker = true;
};
