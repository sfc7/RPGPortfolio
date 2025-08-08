// LJS


#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/GASManager.h"
#include "GameMode/GameManager/LevelManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "GameMode/GameManager/InteractManager.h"
#include "GameMode/GameManager/ItemManager.h"
#include "GameMode/GameManager/QuestManager.h"


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

ULevelManager* UGeneralGameManager::GetLevelManager() const
{
	return GetGameInstance()->GetSubsystem<ULevelManager>();
}

UItemManager* UGeneralGameManager::GetItemManager() const
{
	return GetGameInstance()->GetSubsystem<UItemManager>();
}

UInteractManager* UGeneralGameManager::GetInteractManager() const
{
	return GetGameInstance()->GetSubsystem<UInteractManager>();
}

UQuestManager* UGeneralGameManager::GetQuestManager() const
{
	return GetGameInstance()->GetSubsystem<UQuestManager>();
}

UGASManager* UGeneralGameManager::GetGASManager() const
{
	return GetGameInstance()->GetSubsystem<UGASManager>();
}
