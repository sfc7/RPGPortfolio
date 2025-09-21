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
			UDefaultInventoryTypeStrategy* Strategy = NewObject<UDefaultInventoryTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::Equipment:
		{
			UEquipmentInventoryTypeStrategy* Strategy = NewObject<UEquipmentInventoryTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::Potion:
		{
			UPotionInventoryTypeStrategy* Strategy = NewObject<UPotionInventoryTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::Material:
		{
			UMaterialInventoryTypeStrategy* Strategy = NewObject<UMaterialInventoryTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;

	case EInventoryTypeStrategy::None:
		CurrentInventoryTypeStrategy = nullptr;
		break;

	default:
		{
			UDefaultInventoryTypeStrategy* Strategy = NewObject<UDefaultInventoryTypeStrategy>(this);
			CurrentInventoryTypeStrategy = TScriptInterface<IInventoryTypeStrategy>(Strategy);
		}
		break;
	}
}


TArray<FInventorySlot>& UEquipmentInventoryTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->EquipmentItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UEquipmentInventoryTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->EquipmentItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

TArray<FInventorySlot>& UPotionInventoryTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->PotionItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UPotionInventoryTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->PotionItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

TArray<FInventorySlot>& UMaterialInventoryTypeStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->MaterialItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UMaterialInventoryTypeStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->MaterialItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}
