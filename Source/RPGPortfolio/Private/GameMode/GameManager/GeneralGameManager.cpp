// LJS


#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/LevelManager.h"
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

ULevelManager* UGeneralGameManager::GetLevelManager() const
{
	return GetGameInstance()->GetSubsystem<ULevelManager>();
}

void UGeneralGameManager::ToggleInputMode(const UObject* WorldContextObject, ERPGInputMode InputMode)
{
	APlayerController* PlayerController = nullptr;

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if (!PlayerController) return;

	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;

	switch (InputMode)
	{
	case ERPGInputMode::GameMode:
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
		break;
	case ERPGInputMode::UIMode:
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;
		break;
	default:
		break;
	}
}
