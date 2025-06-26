// LJS


#include "GameMode/GameManager/ItemManager.h"

#include "Component/Player/PlayerInventoryComponent.h"
#include "DataAsset/DataAsset_RPGItemData.h"

UItemManager::UItemManager()
{
	
}

void UItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UItemManager::Deinitialize()
{
	Super::Deinitialize();
}

bool UItemManager::IsInventorySlotEmpty(FInventorySlot InventorySlot)
{
	if (!InventorySlot.ItemDataAsset.IsValid()) return true;

	return false;
}

bool UItemManager::IsInventorySlotNotEmpty(FInventorySlot InventorySlot)
{
	return !IsInventorySlotEmpty(InventorySlot);
}

FInventorySlot UItemManager::MakeItemToAdd(TSoftObjectPtr<UDataAsset_RPGItemData> ItemToAdd, int32 Quantity)
{
	UDataAsset_RPGItemData* Item = ItemToAdd.LoadSynchronous();
	if (!Item) return FInventorySlot();

	
	FInventorySlot MakeInventorySlot;
	MakeInventorySlot.ItemID = Item->GetItemID();
	MakeInventorySlot.Quantity = Quantity;
	MakeInventorySlot.ItemDataAsset = ItemToAdd;
	
	return MakeInventorySlot;
}

