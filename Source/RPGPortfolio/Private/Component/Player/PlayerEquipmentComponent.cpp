// LJS


#include "Component/Player/PlayerEquipmentComponent.h"

#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAttributeSet.h"

UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	InventoryType = EInventoryType::Equipment;

	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetOwner()))
	{
		PlayerInventoryComponentRef = PlayerCharacter->GetPlayerInventoryComponent();
	}
	

}

void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	HelmetSlot = ItemSlots[0];
	ArmorSlot = ItemSlots[1];
	PantsSlot = ItemSlots[2];
	GlovesSlot = ItemSlots[3];
	BootsSlot = ItemSlots[4];
}
void UPlayerEquipmentComponent::ApplyEquipmentItem(FInventorySlot ItemToSet)
{
	UDataAsset_RPGItemData* ItemData = ItemToSet.ItemDataAsset.LoadSynchronous();
	if (!ItemData)
	{
		return;
	}

	UDataAsset_RPGItemData_Equipment* EquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(ItemData);
	if (!EquipmentData)
	{
		return;
	}

	int32 SlotIndex = -1;
	FInventorySlot* TargetSlot = nullptr;

	switch (EquipmentData->EquipmentType)
	{
	case EEquipmentType::Helmet:
		SlotIndex = 0;
		TargetSlot = &HelmetSlot;
		break;
	case EEquipmentType::Armor:
		SlotIndex = 1;
		TargetSlot = &ArmorSlot;
		break;
	case EEquipmentType::Pants:
		SlotIndex = 2;
		TargetSlot = &PantsSlot;
		break;
	case EEquipmentType::Gloves:
		SlotIndex = 3;
		TargetSlot = &GlovesSlot;
		break;
	case EEquipmentType::Boots:
		SlotIndex = 4;
		TargetSlot = &BootsSlot;
		break;
	default:
		break;
	}

	if (SlotIndex != -1 && TargetSlot && TargetSlot->ItemDataAsset.IsValid())
	{
		UDataAsset_RPGItemData* OldItemData = TargetSlot->ItemDataAsset.LoadSynchronous();
		UDataAsset_RPGItemData_Equipment* OldEquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(OldItemData);

		if (OldEquipmentData)
		{
			APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwner());
			if (Player)
			{
				URPGAttributeSet* AttributeSet = Player->GetRPGAttributeSet();
				if (AttributeSet)
				{
					AttributeSet->RemoveEquipmentStats(
						OldEquipmentData->MaxHp,
						OldEquipmentData->MaxMp,
						OldEquipmentData->AttackRate,
						OldEquipmentData->Defense
					);
				}
			}
		}

		if (PlayerInventoryComponentRef)
		{
			PlayerInventoryComponentRef->AddItem(*TargetSlot);
		}
	}

	if (SlotIndex != -1 && TargetSlot)
	{
		*TargetSlot = ItemToSet;
		TargetSlot->SlotIndex = SlotIndex;
		TargetSlot->InventoryRef = this;
		ItemSlots[SlotIndex] = *TargetSlot;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwner());
	if (Player)
	{
		URPGAttributeSet* AttributeSet = Player->GetRPGAttributeSet();
		if (AttributeSet)
		{
			AttributeSet->ApplyEquipmentStats(
				EquipmentData->MaxHp,
				EquipmentData->MaxMp,
				EquipmentData->AttackRate,
				EquipmentData->Defense
			);
		}
	}

	OnEquipmentSlotChangedDelegate.Broadcast();
}
void UPlayerEquipmentComponent::ApplyUnEquipmentItem(FInventorySlot ItemToSet)
{
	UDataAsset_RPGItemData* ItemData = ItemToSet.ItemDataAsset.LoadSynchronous();
	if (!ItemData)
	{
		return;
	}

	UDataAsset_RPGItemData_Equipment* EquipmentData = Cast<UDataAsset_RPGItemData_Equipment>(ItemData);
	if (!EquipmentData)
	{
		return;
	}

	int32 SlotIndex = -1;
	FInventorySlot* TargetSlot = nullptr;

	switch (EquipmentData->EquipmentType)
	{
	case EEquipmentType::Helmet:
		SlotIndex = 0;
		TargetSlot = &HelmetSlot;
		break;
	case EEquipmentType::Armor:
		SlotIndex = 1;
		TargetSlot = &ArmorSlot;
		break;
	case EEquipmentType::Pants:
		SlotIndex = 2;
		TargetSlot = &PantsSlot;
		break;
	case EEquipmentType::Gloves:
		SlotIndex = 3;
		TargetSlot = &GlovesSlot;
		break;
	case EEquipmentType::Boots:
		SlotIndex = 4;
		TargetSlot = &BootsSlot;
		break;
	default:
		break;
	}

	if (SlotIndex != -1 && TargetSlot && TargetSlot->ItemDataAsset.IsValid())
	{
		APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(GetOwner());
		if (Player)
		{
			URPGAttributeSet* AttributeSet = Player->GetRPGAttributeSet();
			if (AttributeSet)
			{
				AttributeSet->RemoveEquipmentStats(
					EquipmentData->MaxHp,
					EquipmentData->MaxMp,
					EquipmentData->AttackRate,
					EquipmentData->Defense
				);
			}
		}

		if (PlayerInventoryComponentRef)
		{
			PlayerInventoryComponentRef->AddItem(*TargetSlot);
		}

		*TargetSlot = FInventorySlot();
		TargetSlot->SlotIndex = SlotIndex;
		TargetSlot->InventoryRef = this;
		ItemSlots[SlotIndex] = *TargetSlot;
	}

	OnEquipmentSlotChangedDelegate.Broadcast();
}

