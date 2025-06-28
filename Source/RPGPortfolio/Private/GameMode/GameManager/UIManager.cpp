// LJS


#include "GameMode/GameManager/UIManager.h"

#include "RPGStructTypes.h"
#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "Engine/StreamableManager.h"

UUIManager::UUIManager()
{
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RPGUIData = LoadObject<UDataAsset_RPGUIData>(nullptr, TEXT("/Game/MyProject/Data/DA_RPGUIData.DA_RPGUIData"));
}

void UUIManager::Deinitialize()
{
	Super::Deinitialize();
}

void UUIManager::ShowUIAsync(const EUICategory& UICategory, UWorld* World)
{
	TSoftClassPtr<UUserWidget> WidgetClass = GetUIWidgetClass(UICategory);
    
	if (WidgetClass.IsNull() || !World)
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		WidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([WidgetClass, World]()
		{
			if (TSubclassOf<UUserWidget> LoadedClass = WidgetClass.Get())
			{
				if (UUserWidget* Widget = CreateWidget<UUserWidget>(World, LoadedClass))
				{
					Widget->AddToViewport();
				}
			}
		})
	);
}

TSoftClassPtr<UUserWidget> UUIManager::GetUIWidgetClass(const EUICategory& UICategory) const
{
	return RPGUIData->GetUIWidgetClass(UICategory);
}

TSubclassOf<UUserWidget> UUIManager::GetLoadingScreen(const ELoadingCategory& LoadingCategory) const
{
	return RPGUIData->GetLoadingScreen(LoadingCategory);
}

TSubclassOf<UUserWidget> UUIManager::GetInventoryUIWidgetClass(const EInventoryUICategory& InventoryUICategory) const
{
	return RPGUIData->GetInventoryUIWidgetClass(InventoryUICategory);
}

void UUIManager::ToggleInputMode(const UObject* WorldContextObject, ERPGInputMode InputMode)
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

UUserWidget* UUIManager::GetWidget() const
{
	return nullptr;
}
