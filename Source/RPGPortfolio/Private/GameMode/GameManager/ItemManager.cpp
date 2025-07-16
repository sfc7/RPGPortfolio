// LJS


#include "GameMode/GameManager/ItemManager.h"

#include "Character/NPC/NPC_HumanNPC.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"

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
    bool bIsPlayerToStore = false;
		
    if (FromContainerInventoryComponent)
    {
        AActor* FromOwner = FromContainerInventoryComponent->GetOwner();
        if (APlayerCharacter_Fighter* PlayerCharacter = Cast<APlayerCharacter_Fighter>(FromOwner))
        {
            if (ToInventoryComponent)
            {
                AActor* ToOwner = ToInventoryComponent->GetOwner();
                if (ANPC_HumanNPC* HumanNPC = Cast<ANPC_HumanNPC>(ToOwner))
                {
                    if (HumanNPC->GetNPCType() == ENPCType::Store)
                    {
                        bIsPlayerToStore = true;
                    }
                }
            }
        }
    }
    
    if (bIsPlayerToStore)
    {
        if (FromContainerInventoryComponent->IsValidSlotIndex(FromIndex))
        {
            FInventorySlot& ItemSlot = FromContainerInventoryComponent->ItemSlots[FromIndex];
            
            if (ItemSlot.ItemDataAsset.IsValid())
            {
                UDataAsset_RPGItemData* ItemData = ItemSlot.ItemDataAsset.LoadSynchronous();
                if (ItemData)
                {
                    int32 SellPrice = ItemData->GoldValue;
                    
                    FromContainerInventoryComponent->SetGold(SellPrice);
                    
                    if (ItemSlot.Quantity > 1)
                    {
                        FInventorySlot ModifiedSlot = ItemSlot;
                        FromContainerInventoryComponent->SetQuantityAtSlot(ModifiedSlot, ItemSlot.Quantity - 1);
                    }
                    else
                    {
                        FromContainerInventoryComponent->RemoveItemToIndex(FromIndex);
                    }
                    
                    return;
                }
            }
        }
    }
    FromContainerInventoryComponent->TransferItem(ToInventoryComponent, FromIndex, ToIndex);
}

int32 UItemManager::GetStaciSize(FInventorySlot TargetSlot)
{
	UDataAsset_RPGItemData* TargetSlotItem = TargetSlot.ItemDataAsset.LoadSynchronous();

	return TargetSlotItem->StackSize;
}
