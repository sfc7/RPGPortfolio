// LJS


#include "Component/Player/PlayerEquipmentComponent.h"

#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAttributeSet.h"

UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	InventoryType = EInventoryType::Equipment;
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

	switch (EquipmentData->EquipmentType)
	{
	case EEquipmentType::Helmet:
		HelmetSlot = ItemToSet;
		break;
	case EEquipmentType::Armor:
		ArmorSlot = ItemToSet;
		break;
	case EEquipmentType::Pants:
		PantsSlot = ItemToSet;
		break;
	case EEquipmentType::Gloves:
		GlovesSlot = ItemToSet;
		break;
	case EEquipmentType::Boots:
		BootsSlot = ItemToSet;
		break;
	default:
		break;
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
}