// LJS


#include "Widget/InventoryWidget.h"

#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Widget/ItemSlotContainer.h"

UInventoryWidget::UInventoryWidget()
{
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (PC)
	{
		ItemSlotContainer->SetInventoryRef(	PC->GetPlayerInventoryComponent());
	}
}
