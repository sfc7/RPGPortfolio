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

void UPlayerInventoryComponent::SetCurrentInventoryStrategy(EInventoryStrategy InventoryStrategyToSet)
{
	switch (InventoryStrategyToSet)
	{
	case EInventoryStrategy::Default:
		{
			UDefaultInventoryStrategy* Strategy = NewObject<UDefaultInventoryStrategy>(this);
			CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(Strategy);
		}
		break;

	case EInventoryStrategy::Equipment:
		{
			UEquipmentInventoryStrategy* Strategy = NewObject<UEquipmentInventoryStrategy>(this);
			CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(Strategy);
		}
		break;

	case EInventoryStrategy::Potion:
		{
			UPotionInventoryStrategy* Strategy = NewObject<UPotionInventoryStrategy>(this);
			CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(Strategy);
		}
		break;

	case EInventoryStrategy::Material:
		{
			UMaterialInventoryStrategy* Strategy = NewObject<UMaterialInventoryStrategy>(this);
			CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(Strategy);
		}
		break;

	case EInventoryStrategy::None:
		CurrentInventoryStrategy = nullptr;
		break;

	default:
		{
			UDefaultInventoryStrategy* Strategy = NewObject<UDefaultInventoryStrategy>(this);
			CurrentInventoryStrategy = TScriptInterface<IInventoryStrategy>(Strategy);
		}
		break;
	}
}


TArray<FInventorySlot>& UEquipmentInventoryStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->EquipmentItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UEquipmentInventoryStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->EquipmentItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

TArray<FInventorySlot>& UPotionInventoryStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->PotionItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UPotionInventoryStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->PotionItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

TArray<FInventorySlot>& UMaterialInventoryStrategy::GetSlots(UInventoryComponent* OwnerInventory)
{
	UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->MaterialItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}

const TArray<FInventorySlot>& UMaterialInventoryStrategy::GetSlots(const UInventoryComponent* OwnerInventory) const
{
	const UPlayerInventoryComponent* PlayerInventory = Cast<UPlayerInventoryComponent>(OwnerInventory);
	if (PlayerInventory)
	{
		return PlayerInventory->MaterialItemSlots;
	}

	return OwnerInventory->DefaultItemSlots;
}
