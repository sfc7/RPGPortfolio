// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "PlayerCombatGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCombatGameplayAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayerCombatGameplayAbility();
protected:
	
	UFUNCTION()
	void FindNearestEnemyBeforeAttack(float BoxExtent);

	UFUNCTION(BlueprintCallable)
	void RotateTargetTickBeforeAttack(float DeltaTime);

	UPROPERTY()
	AActor* RotateTargetActor;
};
