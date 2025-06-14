// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class APlayerCharacterBase;
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
	APlayerCharacterBase* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	APlayerCharacterController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakePlayerComboDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag,int32 _UsedComboCount);

	UFUNCTION(BlueprintPure, Category = "GA")
	FGameplayEffectSpecHandle MakePlayerBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag);

	UFUNCTION(BlueprintCallable, Category = "GA")
	bool GetAbilityRemaningCooldownByTag(FGameplayTag CooldownTag, float& TotalCooldownTime, float& RemaningCooldownTime);
	
private:
	TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;
	TWeakObjectPtr<APlayerCharacterController> PlayerCharacterController;
};
