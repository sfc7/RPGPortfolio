// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "RPGGC_Player_Defense.generated.h"

/**
 * ARPGGC_Player_Defense
 * 
 * 방어 시 지속적으로 이펙트를 적용할 GameplayCue(Actor)
 */
UCLASS()
class RPGPORTFOLIO_API ARPGGC_Player_Defense : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	ARPGGC_Player_Defense();

protected:
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	virtual bool OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;
};
