// LJS


#include "DataAsset/DataAsset_RPGUIData.h"

UDataAsset_RPGUIData::UDataAsset_RPGUIData(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

TSoftClassPtr<UUserWidget> UDataAsset_RPGUIData::GetUIWidgetClass(const EUICategory& UICategory) const
{
	switch (UICategory)
	{
	case EUICategory::GameOverUI:
		return GameOverUI.UIWidgetClass;
	case EUICategory::VictoryUI:
		return VictoryUI.UIWidgetClass;
	case EUICategory::PauseMenuUI:
		return PauseMenuUI.UIWidgetClass;
	case EUICategory::TitleMenuUI:
		return TitleMenuUI.UIWidgetClass;
	case EUICategory::InventoryUI:
		return InventoryUI.UIWidgetClass;
	case EUICategory::QuestUI:
		return PlayerQuestUI.UIWidgetClass;
	default:
		return nullptr;
	}
}

TSubclassOf<UUserWidget> UDataAsset_RPGUIData::GetLoadingScreen(const ELoadingCategory& LoadingCategory) const
{
	switch (LoadingCategory)
	{
	case ELoadingCategory::Town:
		return LoadingScreen_Town.LoadingScreen;
	case ELoadingCategory::Dungeon:
		return LoadingScreen_Dungeon.LoadingScreen;
	default:
		return nullptr;
	}
}

TSubclassOf<UUserWidget> UDataAsset_RPGUIData::GetInventoryUIWidgetClass(const EInventoryUICategory& InventoryUICategory) const
{
	switch (InventoryUICategory)
	{
	case EInventoryUICategory::ItemSlot:
		return ItemSlotUI.InventoryUIWidgetClass;
	case EInventoryUICategory::PotionSlot:
		return PotionSlotUI.InventoryUIWidgetClass;
	default:
		return nullptr;
	}
}

TSubclassOf<UUserWidget> UDataAsset_RPGUIData::GetInteractUIWidgetClass(const EInteractUICategory& InteractUICategory) const
{
	switch (InteractUICategory)
	{
	case EInteractUICategory::Storage:
		return StorageUI.InteractUIWidgetClass;
	case EInteractUICategory::Store:
		return StoreUI.InteractUIWidgetClass;
	default:
		return nullptr;
	}
}

TSubclassOf<UUserWidget> UDataAsset_RPGUIData::GetQuestUIWidgetClass(const EQuestUICategory& QuestUICategory) const
{
	switch (QuestUICategory)
	{
	case EQuestUICategory::NPC:
		return NPCQuestUI.InteractUIWidgetClass;
	case EQuestUICategory::Reward:
		return NPCQuesRewardsUI.InteractUIWidgetClass;
	default:
		return nullptr;
	}
}

