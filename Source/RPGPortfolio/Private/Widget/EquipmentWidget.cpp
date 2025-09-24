// LJS


#include "Widget/EquipmentWidget.h"
#include "Widget/ItemSlotContainer.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Component/Player/PlayerEquipmentComponent.h"

UEquipmentWidget::UEquipmentWidget()
{
}

void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (PC)
	{
		UPlayerEquipmentComponent* EquipmentInventory = PC->GetPlayerEquipmentComponent();

		if (EquipmentInventory)
		{
			EquipmentSlotContainer->SetInventoryRef(EquipmentInventory);
			EquipmentInventory->OnEquipmentSlotChangedDelegate.AddDynamic(this, &UEquipmentWidget::OnEquipmentChanged);
		}
	}
}
void UEquipmentWidget::OnEquipmentChanged()
{
	if (EquipmentSlotContainer)
	{
		EquipmentSlotContainer->RefreshSlots();
	}
}
