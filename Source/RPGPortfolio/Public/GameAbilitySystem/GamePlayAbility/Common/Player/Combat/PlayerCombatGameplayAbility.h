// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "PlayerCombatGameplayAbility.generated.h"

class URPGAT_Player_RotateTarget;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCombatGameplayAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayerCombatGameplayAbility();

	UFUNCTION(BlueprintCallable)
	void RotateTargetTickBeforeAttack(float DeltaTime);
	
protected:
	//Rotate
	UFUNCTION()
	bool FindNearestEnemyBeforeAttack(float BoxExtent);

	UPROPERTY()
	AActor* RotateTargetActor;

	UPROPERTY()
	FRotator FindRototation;
	//Rotate
};
