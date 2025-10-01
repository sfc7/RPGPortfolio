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
	if (IsValid(PC))
	{
		UPlayerEquipmentComponent* EquipmentInventory = PC->GetPlayerEquipmentComponent();
		
		if (IsValid(EquipmentInventory))
		{
			EquipmentSlotContainer->SetInventoryRef(EquipmentInventory);
			EquipmentInventory->OnEquipmentSlotChangedDelegate.AddDynamic(this, &UEquipmentWidget::OnEquipmentChanged);
		}
	}
}
void UEquipmentWidget::OnEquipmentChanged()
{
	//@ 장비 변경시 슬롯들 재설정
	if (IsValid(EquipmentSlotContainer))
	{
		EquipmentSlotContainer->RefreshSlots();
	}
}
