// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class APlayerCharacter;
class APlayerCharacterController;
class UPlayerCombatComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerGameplayAbility : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "GA")
	APlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	APlayerCharacterController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();
	
private:
	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	TWeakObjectPtr<APlayerCharacterController> PlayerCharacterController;
};
