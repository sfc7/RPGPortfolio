// LJS

#include "Component/InventoryComponent.h"

#include "GameMode/GameManager/ItemManager.h"
#include "Kismet/KismetArrayLibrary.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "GameMode/GameManager/UIManager.h"
#include "Component/Player/PlayerEquipmentComponent.h"
#include "Item/InventoryTypeStrategy.h"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManager>();
	
	if(!CurrentInventoryStrategy)
	{
		UDefaultInventoryStrategy* DefaultStrategy = NewObject<UDefaultInventoryStrategy>(this);
		CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(DefaultStrategy);
	}
	
	SetupSlots(SlotAmounts);
}

void UInventoryComponent::SetupSlots(int32 SlotAmountstoSetup)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	for (int Index = 0; Index < SlotAmountstoSetup; Index++)
	{
		FInventorySlot InventorySlot;
		InventorySlot.SlotIndex = Index;
		InventorySlot.InventoryRef = this;
		Slots.Add(InventorySlot);
	}
}

bool UInventoryComponent::AddItem(FInventorySlot ItemToAdd)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	FInventorySlot CurrentItemToAdd= ItemToAdd;
	UDataAsset_RPGItemData* CurrentItemData = CurrentItemToAdd.ItemDataAsset.LoadSynchronous();
	bool IsStackable = CurrentItemData->IsStackable();
	int32 CurrentItemStackSize = CurrentItemData->StackSize;
	
	if (IsStackable)
	{
		for (FInventorySlot& TargetSlot : Slots)
		{
			bool IsStackableAndIsEqualAndHaveSpace = ItemManager->IsStackableAndIsEqualAndHaveSpace(TargetSlot, CurrentItemToAdd);
			if (IsStackableAndIsEqualAndHaveSpace)
			{
				int32 TotalQuantity = TargetSlot.Quantity + CurrentItemToAdd.Quantity;

				if (TotalQuantity > CurrentItemStackSize)
				{
					SetQuantityAtSlot(TargetSlot, CurrentItemStackSize);
					CurrentItemToAdd.Quantity = TotalQuantity -	CurrentItemStackSize;
				}
				else
				{
					SetQuantityAtSlot(TargetSlot, TotalQuantity);
					return true;
				}
			}
		}

		FInventorySlot FindInventorySlot;
		while (FindEmptySlot((FindInventorySlot)))
		{
			if (CurrentItemToAdd.Quantity <= CurrentItemStackSize)
			{
				SetItem(FindInventorySlot, CurrentItemToAdd);
				return true;
			}
			else
			{
				FInventorySlot TempItemToAdd = CurrentItemToAdd;
				TempItemToAdd.Quantity = CurrentItemStackSize;
				SetItem(FindInventorySlot, TempItemToAdd);

				int32 RemainQuantity = CurrentItemToAdd.Quantity - CurrentItemStackSize;
				CurrentItemToAdd.Quantity = RemainQuantity;
			}
		}
	}
	else
	{
		FInventorySlot FindInventorySlot;
		if (FindEmptySlot((FindInventorySlot)))
		{
			SetItem(FindInventorySlot, ItemToAdd);
			return true;
		}
	}
	
	return false;
}

bool UInventoryComponent::AddItemToIndex(FInventorySlot ItemToAdd, int32 ToIndex, bool& OutAreAllItemAdded)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	if (IsValidSlotIndex(ToIndex))
	{
		if (ItemManager->IsInventorySlotEmpty(Slots[ToIndex]))
		{
			SetItem(Slots[ToIndex], ItemToAdd);
						
			OutAreAllItemAdded = true;
			return true;
		}
		else
		{
			StackItemOnTransfer(Slots[ToIndex], ItemToAdd, OUT OutAreAllItemAdded);
			return true;
		}
	}
	else
	{
		OutAreAllItemAdded = false;
		return false;
	}
}

bool UInventoryComponent::RemoveItemToIndex(int32 ToIndex)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	if (IsValidSlotIndex(ToIndex))
	{
		FInventorySlot EmptyInventorySlot;
		EmptyInventorySlot.ItemID = FName(TEXT("None"));
		EmptyInventorySlot.Quantity = 0;
		
		SetItem(Slots[ToIndex], EmptyInventorySlot);

		return true;
	}
	else
	{
		return false;
	}
}

FInventorySlot UInventoryComponent::SetQuantityAtSlot(FInventorySlot& TargetSlot, int32 QuantityToSet)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	Slots[TargetSlot.SlotIndex].Quantity = QuantityToSet;

	OnInventorySlotChangedDelegate.Broadcast(Slots[TargetSlot.SlotIndex]);

	return Slots[TargetSlot.SlotIndex];
}

bool UInventoryComponent::TransferItem(UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	if (this == ToInventoryComponent && FromIndex == ToIndex)
	{
		return false;
	}
	else
	{
		if (IsValid(ToInventoryComponent) && IsValidSlotIndex(FromIndex) && (ToInventoryComponent->IsValidSlotIndex(ToIndex) || ToIndex == -1)) // -1 : 다른 인벤토리로 옮길경우
		{
			if (ToIndex == -1)
			{
				UE_LOG(LogTemp, Error, TEXT("Inventory Item Index is invalid"));
			}
			else
			{
				bool AreAllItemsAdded;
				bool SuccessAdd = ToInventoryComponent->AddItemToIndex(Slots[FromIndex], ToIndex, AreAllItemsAdded);
				if (SuccessAdd)
				{
					if (AreAllItemsAdded)
					{
						bool RemoveSuccess = RemoveItemToIndex(FromIndex);
						return RemoveSuccess;
					}
					else
					{
						return true;	
					}
				}
				else
				{
					return false;
				}
			}
		}

		return false;
	}
}

bool UInventoryComponent::IsValidSlotIndex(int32 FindIndex)
{
	return GetCurrentItemSlots().IsValidIndex(FindIndex);
}

bool UInventoryComponent::StackItemOnTransfer(FInventorySlot TargetSlot, FInventorySlot FromSlot, bool& OutAreAllItemAdded)
{
	bool IsStackableAndIsEqualAndHaveSpace = ItemManager->IsStackableAndIsEqualAndHaveSpace(TargetSlot, FromSlot);
	int32 ItemStackSize = ItemManager->GetStackSize(TargetSlot);

	if (IsStackableAndIsEqualAndHaveSpace)
	{
		int32 TotalQuantity = TargetSlot.Quantity + FromSlot.Quantity;
		
		if (TotalQuantity > ItemStackSize)
		{
			SetQuantityAtSlot(TargetSlot, ItemStackSize);
			SetQuantityAtSlot(FromSlot, TotalQuantity -	ItemStackSize);
			
			OutAreAllItemAdded = false;
			return false;
		}
		else
		{
			SetQuantityAtSlot(TargetSlot, TotalQuantity);
			
			OutAreAllItemAdded = true;
			return true;
		}   
	}
	else
	{
		SwapIndex(TargetSlot, FromSlot);
		OutAreAllItemAdded = false;
	}

	OutAreAllItemAdded = false;
	return false;
}

void UInventoryComponent::SwapIndex(FInventorySlot TargetSlot, FInventorySlot FromSlot)
{
	SetItem(TargetSlot, FromSlot);
	FromSlot.InventoryRef->SetItem(FromSlot, TargetSlot);
}

void UInventoryComponent::EquipItem(FInventorySlot FromSlot)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	int32 FromIndex = FromSlot.SlotIndex;	

	if (Slots.IsValidIndex(FromIndex))
	{
		if (Slots[FromIndex].ItemDataAsset->ItemType != EItemType::Equipment) return;
		
		if (InventoryType != EInventoryType::PlayerInventory) return;

		if (IsValid(EquipmentComponentRef))
		{
			EquipmentComponentRef->ApplyEquipmentItem(FromSlot);
			RemoveItemToIndex(FromIndex);
		}
	}
}

void UInventoryComponent::UnEquipItem(FInventorySlot FromSlot)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	int32 FromIndex = FromSlot.SlotIndex;	

	if (Slots.IsValidIndex(FromIndex))
	{
		if (Slots[FromIndex].ItemDataAsset->ItemType != EItemType::Equipment) return;
		
		if (InventoryType != EInventoryType::Equipment) return;

		if (IsValid(EquipmentComponentRef))
		{
			EquipmentComponentRef->ApplyUnEquipmentItem(FromSlot);
		}
	}
}

void UInventoryComponent::SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	int32 TargetIndex = TargetSlot.SlotIndex;	

	if (Slots.IsValidIndex(TargetIndex))
	{
		Slots[TargetIndex] = ItemToSet;
		Slots[TargetIndex].SlotIndex = TargetIndex;
		Slots[TargetIndex].InventoryRef = this;
		
		OnInventorySlotChangedDelegate.Broadcast(Slots[TargetIndex]);
		
		if (InventoryType == EInventoryType::Potion)
		{
			OnPotionBarSlotChangedDelegate.Broadcast();
		}
		else if (InventoryType == EInventoryType::Equipment)
		{
			OnEquipmentSlotChangedDelegate.Broadcast();
		}
	}
}

void UInventoryComponent::SetGold(int32 GoldAmount)
{
	Gold += GoldAmount;
	OnGoldChangedDelegate.Broadcast(Gold);
}

bool UInventoryComponent::FindEmptySlot(FInventorySlot& OutEmptySlot)
{
	TArray<FInventorySlot>& Slots = GetCurrentItemSlots();
	for (FInventorySlot& ItemSlot : Slots)
	{
		bool IsEmpty = GetWorld()->GetGameInstance()->GetSubsystem<UItemManager>()->IsInventorySlotEmpty(ItemSlot);
		if (IsEmpty)
		{
			OutEmptySlot = ItemSlot;
			return true;
		}
	}
	return false;
}

TArray<FInventorySlot>& UInventoryComponent::GetCurrentItemSlots()
{
	if (CurrentInventoryStrategy)
	{
		return CurrentInventoryStrategy->GetSlots(this);
	}
	return DefaultItemSlots;
}

const TArray<FInventorySlot>& UInventoryComponent::GetCurrentItemSlots() const
{
	if (CurrentInventoryStrategy)
	{
		return CurrentInventoryStrategy->GetSlots(this);
	}
	return DefaultItemSlots;
}

void UInventoryComponent::SetDefaultInventoryStrategy()
{
	if(!CurrentInventoryStrategy)
	{
		UDefaultInventoryStrategy* DefaultInventoryStrategy = NewObject<UDefaultInventoryStrategy>(this);
		CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(DefaultInventoryStrategy);
	}
}





///////// UDefaultInventoryStrategy


TArray<FInventorySlot>& UDefaultInventoryStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	check(OwnerInventory);
	return OwnerInventory->DefaultItemSlots;
}
const TArray<FInventorySlot>& UDefaultInventoryStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	check(OwnerInventory);
	return OwnerInventory->DefaultItemSlots;
}