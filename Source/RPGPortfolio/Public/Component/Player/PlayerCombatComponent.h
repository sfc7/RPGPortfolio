// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/CombatComponentBase.h"
#include "PlayerCombatComponent.generated.h"

class APlayerWeapon;

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	APlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	APlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel() const;
	
	virtual void OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage);
	virtual void OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage);
	
};
