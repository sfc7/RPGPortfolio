// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_HeavyAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_HeavyAttack : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_HeavyAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<int32, UAnimMontage*> HeavyAttackMontages;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnEndAbilityCallback();
};
