// LJS


#include "GameMode/DungeonGameMode.h"

#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/GeneralGameManager.h"


void ADungeonGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ADungeonGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime += DeltaTime;

	if (GameTime >= 5.f && !flagspawn)
	{
		GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->SpawnBossMonster();
		flagspawn = true;
	}
}