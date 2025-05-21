// LJS

#pragma once

#include "CoreMinimal.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "RPGStructTypes.h"
#include "PlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API APlayerWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FPlayerWeaponData PlayerWeaponData;
};
