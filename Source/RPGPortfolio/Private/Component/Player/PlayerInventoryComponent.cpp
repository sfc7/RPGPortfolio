// LJS


#include "Component/Player/PlayerInventoryComponent.h"

#include "GameMode/GameManager/ItemManager.h"
#include "Kismet/KismetArrayLibrary.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "GameMode/GameManager/UIManager.h"
#include "Component/Player/PlayerEquipmentComponent.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetOwner()))
	{
		PlayerEquipmentComponentRef = PlayerCharacter->GetPlayerEquipmentComponent();
	}
	
	ItemManager = GetWorld()->GetGameInstance()->GetSubsystem<UItemManager>();
	SetupSlots(SlotAmounts);
}

void UPlayerInventoryComponent::SetupSlots(int32 SlotAmountstoSetup)
{
	for (int Index = 0; Index<SlotAmountstoSetup; Index++)
	{
		FInventorySlot InventorySlot;
		InventorySlot.SlotIndex = Index;
		InventorySlot.InventoryRef = this;
		ItemSlots.Add(InventorySlot);
	}
}

bool UPlayerInventoryComponent::AddItem(FInventorySlot ItemToAdd)
{
	FInventorySlot CurrentItemToAdd= ItemToAdd;
	UDataAsset_RPGItemData* CurrentItemData = CurrentItemToAdd.ItemDataAsset.LoadSynchronous();
	bool IsStackable = CurrentItemData->IsStackable();
	int32 CurrentItemStackSize = CurrentItemData->StackSize;

	if (IsStackable)
	{
		for (FInventorySlot TargetSlot : ItemSlots)
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

bool UPlayerInventoryComponent::AddItemToIndex(FInventorySlot ItemToAdd, int32 ToIndex, bool& OutAreAllItemAdded)
{
	if (IsValidSlotIndex(ToIndex))
	{
		if (ItemManager->IsInventorySlotEmpty(ItemSlots[ToIndex]))
		{
			SetItem(ItemSlots[ToIndex], ItemToAdd);
						
			OutAreAllItemAdded = true;
			return true;
		}
		else
		{
			StackItemOnTransfer(ItemSlots[ToIndex], ItemToAdd, OUT OutAreAllItemAdded);
			
			return true;
		}
	}
	else
	{
		OutAreAllItemAdded = false;
		return false;
	}
}

bool UPlayerInventoryComponent::RemoveItemToIndex(int32 ToIndex)
{
	if (IsValidSlotIndex(ToIndex))
	{
		FInventorySlot EmptyInventorySlot;
		EmptyInventorySlot.ItemID = FName(TEXT("None"));
		EmptyInventorySlot.Quantity = 0;
		
		SetItem(ItemSlots[ToIndex], EmptyInventorySlot);

		return true;
	}
	else
	{
		return false;
	}
}

FInventorySlot UPlayerInventoryComponent::SetQuantityAtSlot(FInventorySlot& TargetSlot, int32 QuantityToSet)
{
	ItemSlots[TargetSlot.SlotIndex].Quantity = QuantityToSet;

	OnInventorySlotChangedDelegate.Broadcast(ItemSlots[TargetSlot.SlotIndex]);

	return ItemSlots[TargetSlot.SlotIndex];
}

bool UPlayerInventoryComponent::TransferItem(UPlayerInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex)
{
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
				bool SuccessAdd = ToInventoryComponent->AddItemToIndex(ItemSlots[FromIndex], ToIndex, AreAllItemsAdded);
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

bool UPlayerInventoryComponent::IsValidSlotIndex(int32 FindIndex)
{
	return ItemSlots.IsValidIndex(FindIndex) ? true : false;
}

bool UPlayerInventoryComponent::StackItemOnTransfer(FInventorySlot TargetSlot, FInventorySlot FromSlot, bool& OutAreAllItemAdded)
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

void UPlayerInventoryComponent::SwapIndex(FInventorySlot TargetSlot, FInventorySlot FromSlot)
{
	SetItem(TargetSlot, FromSlot);

	FromSlot.InventoryRef->SetItem(FromSlot, TargetSlot);
}

void UPlayerInventoryComponent::EquipItem(FInventorySlot FromSlot)
{
	int32 FromIndex = FromSlot.SlotIndex;	

	if (ItemSlots.IsValidIndex(FromIndex))
	{
		if (ItemSlots[FromIndex].ItemDataAsset->ItemType != EItemType::Equipment) return;
		
		if (InventoryType != EInventoryType::PlayerInventory) return;

		if (IsValid(PlayerEquipmentComponentRef))
		{
			PlayerEquipmentComponentRef->ApplyEquipmentItem(FromSlot);
			RemoveItemToIndex(FromIndex);
		}
	}
}

void UPlayerInventoryComponent::UnEquipItem(FInventorySlot FromSlot)
{
	int32 FromIndex = FromSlot.SlotIndex;	

	if (ItemSlots.IsValidIndex(FromIndex))
	{
		if (ItemSlots[FromIndex].ItemDataAsset->ItemType != EItemType::Equipment) return;
		
		if (InventoryType != EInventoryType::Equipment) return;

		if (IsValid(PlayerEquipmentComponentRef))
		{
			PlayerEquipmentComponentRef->ApplyUnEquipmentItem(FromSlot);
			
		}
	}
}

void UPlayerInventoryComponent::SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet)
{
	int32 TargetIndex = TargetSlot.SlotIndex;	

	if (ItemSlots.IsValidIndex(TargetIndex))
	{
		ItemSlots[TargetIndex] = ItemToSet;
		ItemSlots[TargetIndex].SlotIndex = TargetIndex;
		ItemSlots[TargetIndex].InventoryRef = this;
		
		OnInventorySlotChangedDelegate.Broadcast(ItemSlots[TargetIndex]);
		
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

void UPlayerInventoryComponent::SetGold(int32 GoldAmount)
{
	PlayerGold += GoldAmount;

	OnGoldChangedDelegate.Broadcast(PlayerGold);
}

bool UPlayerInventoryComponent::FindEmptySlot(FInventorySlot& OutEmptySlot)
{
	for (FInventorySlot& ItemSlot : ItemSlots)
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
