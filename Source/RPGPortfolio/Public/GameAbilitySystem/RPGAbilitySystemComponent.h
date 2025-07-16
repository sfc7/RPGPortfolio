// LJS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

class URPGSaveGame;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& _InputTag);
	void OnAbilityInputReleased(const FGameplayTag& _InputTag);

	UFUNCTION(BlueprintCallable)
	bool ActivateGamePlayAbilityByTag(FGameplayTag _ActivateAbilityTag);

	FGameplayAbilitySpec* FindActiveAbilityByTag(FGameplayTag AbilityTag);

	UFUNCTION(BlueprintPure, Category = "Skills")
	TArray<FGameplayAbilitySpec> GetAllSkillAbilities() const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveDynamicAbilitiesToSaveGame(URPGSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadDynamicAbilitiesFromSaveGame(URPGSaveGame* SaveGame);

private:
	bool IsDynamicAbility(const FGameplayAbilitySpec& Spec) const;
};
