// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/RPGGA_SpawnWeaponBase.h"
#include "RPGGA_Player_SpawnWeapon.generated.h"

class APlayerCharacterBase;
class APlayerWeapon;
/**
 * URPGGA_Player_SpawnWeapon
 * 
 * 플레이어용 장비 장착 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_SpawnWeapon : public URPGGA_SpawnWeaponBase
{
	GENERATED_BODY()
public:
	URPGGA_Player_SpawnWeapon();
};
