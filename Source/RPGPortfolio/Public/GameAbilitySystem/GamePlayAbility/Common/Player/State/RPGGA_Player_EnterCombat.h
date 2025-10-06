// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "RPGGA_Player_EnterCombat.generated.h"

/**
 * URPGGA_Player_EnterCombat
 * 
 * 플레이어 Idle이 전투로 돌입될 떄 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_EnterCombat : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_EnterCombat();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

		
};
