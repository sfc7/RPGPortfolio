// LJS


#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/UIManager.h"

void UGeneralGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGeneralGameManager::Deinitialize()
{
	Super::Deinitialize();
}

UDungeonProgressManager* UGeneralGameManager::GetDungeonProgressManager() const
{
	return GetGameInstance()->GetSubsystem<UDungeonProgressManager>();
}

UUIManager* UGeneralGameManager::GetUIManager() const
{
	return GetGameInstance()->GetSubsystem<UUIManager>();
}
