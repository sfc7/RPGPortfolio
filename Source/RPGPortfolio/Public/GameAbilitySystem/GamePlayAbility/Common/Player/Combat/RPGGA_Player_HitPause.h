// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"
#include "RPGGA_Player_HitPause.generated.h"

/**
 * URPGGA_Player_HitPause
 * 
 * 플레이어 피격 시 살짝 멈추는 연출용 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_HitPause : public UPlayerCombatGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_HitPause();

};
