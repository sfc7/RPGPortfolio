// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_HitReact.generated.h"

/**
 * URPGGA_Player_HitPause
 * 
 * 플레이어 피격 시 Montage 실행 관련 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_HitReact : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_HitReact();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 정면 피격용 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Front;

	//@ 후면 피격용 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Back;

	//@ 좌측 피격용 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Left;

	//@ 우측 피격용 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* HitReactMontage_Right;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();
};
