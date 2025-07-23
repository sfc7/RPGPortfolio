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
		EquipmentSlotContainer->SetInventoryRef(PC->GetPlayerEquipmentComponent());
	}
}
