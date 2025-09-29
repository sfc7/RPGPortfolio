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
		EquipmentComponentRef = PlayerCharacter->GetPlayerEquipmentComponent();
	}
}

void UPlayerInventoryComponent::SetupSlots(int32 SlotAmountstoSetup)
{
	Super::SetupSlots(SlotAmountstoSetup);
	
	for (int Index = 0; Index<SlotAmountstoSetup; Index++)
	{
		FInventorySlot InventorySlot;
		InventorySlot.SlotIndex = Index;
		InventorySlot.InventoryRef = this;
		MaterialItemSlots.Add(InventorySlot);
		PotionItemSlots.Add(InventorySlot);
		EquipmentItemSlots.Add(InventorySlot);
	}
}

void UPlayerInventoryComponent::SetCurrentInventoryTypeStrategy(EInventoryTypeStrategy InventoryTypeStrategyToSet)
{
	switch (InventoryTypeStrategyToSet)
	{
	case EInventoryTypeStrategy::Default:
		{
			UDefaultTypeStrategy* Strategy = NewObject<UDefaultTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::Equipment:
		{
			UEquipmentTypeStrategy* Strategy = NewObject<UEquipmentTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::Potion:
		{
			UPotionTypeStrategy* Strategy = NewObject<UPotionTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::Material:
		{
			UMaterialTypeStrategy* Strategy = NewObject<UMaterialTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::None:
		CurrentInventoryTypeStrategy = nullptr;
		break;

	default:
		{
			UDefaultTypeStrategy* Strategy = NewObject<UDefaultTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;
	}
}

void UPlayerInventoryComponent::SetCurrentInventorySituationStrategy(EInventorySituationStrategy InventorySituationStrategyToSet)
{
	switch (InventorySituationStrategyToSet)
	{
	case EInventorySituationStrategy::Default:
		{
			UDefaultSituationStrategy* Strategy = NewObject<UDefaultSituationStrategy>(this);
			CurrentInventorySituationStrategy = TScriptInterface<IInventorySituationStrategy>(Strategy);
		}
		break;

	case EInventorySituationStrategy::InOpenEquipment:
		{
			UInOpenEquipmentStrategy* Strategy = NewObject<UInOpenEquipmentStrategy>(this);
			CurrentInventorySituationStrategy = TScriptInterface<IInventorySituationStrategy>(Strategy);
		}
		break;

	case EInventorySituationStrategy::InOpenStore:
		{
			UInOpenStoreStrategy* Strategy = NewObject<UInOpenStoreStrategy>(this);
			CurrentInventorySituationStrategy = TScriptInterface<IInventorySituationStrategy>(Strategy);
		}
		break;

	case EInventorySituationStrategy::None:
		{
			CurrentInventorySituationStrategy = nullptr;
		}
		break;
		
	default:
		{
			UDefaultSituationStrategy* Strategy = NewObject<UDefaultSituationStrategy>(this);
			CurrentInventorySituationStrategy = TScriptInterface<IInventorySituationStrategy>(Strategy);
		}
		break;
	}
}


/* UInventoryTypeStrategy */

TArray<FInventorySlot>& UEquipmentTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->EquipmentItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UEquipmentTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->EquipmentItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

TArray<FInventorySlot>& UPotionTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->PotionItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UPotionTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->PotionItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

TArray<FInventorySlot>& UMaterialTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->MaterialItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UMaterialTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->MaterialItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

/* UEquipmentInventorySituationStrategy */


// 상점이 열린 상태에서의 DoubleClick
void UInOpenStoreStrategy::HandleItemDoubleClick(UInventoryComponent* Inventory, FInventorySlot& SlotData)
{	
	if (SlotData.ItemDataAsset->ItemType == EItemType::Equipment)
	{
		if (Inventory->GetInventoryType() == EInventoryType::PlayerInventory)
		{
			Inventory->TrySellItem(SlotData);
		}
	}
}

// 장비창이 열린 상태에서의 DoubleClick
void UInOpenEquipmentStrategy::HandleItemDoubleClick(UInventoryComponent* Inventory, FInventorySlot& SlotData)
{
	if (SlotData.ItemDataAsset->ItemType == EItemType::Equipment)
	{
		if (Inventory->GetInventoryType() == EInventoryType::PlayerInventory)
		{
			Inventory->EquipItem(SlotData);
		}
	}
}

void UInOpenEquipmentStrategy::HandleItemRightClick(UInventoryComponent* Inventory, FInventorySlot& SlotData)
{
	if (SlotData.ItemDataAsset->ItemType == EItemType::Equipment)
	{
		if (Inventory->GetInventoryType() == EInventoryType::PlayerInventory)
		{
			Inventory->EquipItem(SlotData);
		}
	}
}

void UInOpenStoreStrategy::HandleItemRightClick(UInventoryComponent* Inventory, FInventorySlot& SlotData)
{
	
}