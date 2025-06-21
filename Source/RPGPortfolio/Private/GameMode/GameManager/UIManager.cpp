// LJS


#include "GameMode/GameManager/UIManager.h"
#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "Engine/StreamableManager.h"

UUIManager::UUIManager()
{
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RPGUIData = LoadObject<UDataAsset_RPGUIData>(nullptr, TEXT("/Game/MyProject/Widget/Data/DA_RPGUIData.DA_RPGUIData"));
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
