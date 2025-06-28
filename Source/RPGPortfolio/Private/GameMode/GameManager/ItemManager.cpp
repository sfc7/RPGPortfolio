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

void UItemManager::SetSlotIndex(FInventorySlot InventorySlottoSet, int32 Index)
{
	InventorySlottoSet.SlotIndex = Index;
}

bool UItemManager::IsStackableAndIsEqualAndHaveSpace(FInventorySlot& TargetSlot, FInventorySlot& SlotToStack)
{
	bool IsEmpty = IsInventorySlotEmpty(TargetSlot);
	
	if (IsEmpty)
	{
		 return false;
	}
	else
	{
		UDataAsset_RPGItemData* TargetSlotItem = TargetSlot.ItemDataAsset.LoadSynchronous();
		bool TargetIsStackalbe = TargetSlotItem->IsStackable();
		if (TargetIsStackalbe)
		{
			if (TargetSlot.ItemID == SlotToStack.ItemID)
			{
				if (TargetSlotItem->StackSize > TargetSlot.Quantity)
				{
					return true;
				}
			}
		}		
	}

	return false;
}

void UItemManager::OnInventorySlotDrop(UPlayerInventoryComponent* FromContainerInventoryComponent, UPlayerInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex) const
{
	FromContainerInventoryComponent->TransferItem(ToInventoryComponent, FromIndex, ToIndex);
}

int32 UItemManager::GetStaciSize(FInventorySlot TargetSlot)
{
	UDataAsset_RPGItemData* TargetSlotItem = TargetSlot.ItemDataAsset.LoadSynchronous();

	return TargetSlotItem->StackSize;
}
