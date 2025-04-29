// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "Player_EnterCombat.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayer_EnterCombat : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayer_EnterCombat();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

		
};
