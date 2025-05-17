// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/RPGGA_SpawnWeaponBase.h"
#include "RPGGA_Player_SpawnWeapon.generated.h"

class APlayerCharacter;
class APlayerWeapon;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_SpawnWeapon : public URPGGA_SpawnWeaponBase
{
	GENERATED_BODY()
public:
	URPGGA_Player_SpawnWeapon();
};
