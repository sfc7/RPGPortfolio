// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameMode/RPGGameModeBase.h"
#include "DungeonGameMode.generated.h"

UCLASS()
class RPGPORTFOLIO_API ADungeonGameMode : public ARPGGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	float GameTime;

	bool flagspawn;
};
