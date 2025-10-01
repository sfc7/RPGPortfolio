// LJS


#include "GameMode/GameManager/UIManager.h"

#include "RPGStructTypes.h"
#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"
#include "Component/InventoryComponent.h"
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
	const TSoftClassPtr<UUserWidget> WidgetClass = GetUIWidgetClass(UICategory);
    
	if (WidgetClass.IsNull() || !World) return;

	// 위젯 클래스 비동기 로딩
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

TSubclassOf<UUserWidget> UUIManager::GetInteractWidgetClass(const EInteractUICategory& InteractUICategory) const
{
	return RPGUIData->GetInteractUIWidgetClass(InteractUICategory);
}

TSubclassOf<UUserWidget> UUIManager::GetQuestWidgetClass(const EQuestUICategory& QuestUICategory) const
{
	return RPGUIData->GetQuestUIWidgetClass(QuestUICategory);
}

void UUIManager::ToggleInputMode(const UObject* WorldContextObject, ERPGInputMode InputMode)
{
	if (!IsValid(WorldContextObject)) return;
	
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;

	APlayerController* const PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController)) return;

	// 입력 모드 설정
	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;

	// 입력 모드에 따른 처리
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