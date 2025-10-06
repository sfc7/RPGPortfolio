// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_Death.generated.h"

/**
 * URPGGA_Player_Death
 * 
 * 플레이어 사망 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Death : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Death();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 죽음 애니메이션 완료 콜백 - 어빌리티 종료
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 사망 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* DeathMontage;
};
