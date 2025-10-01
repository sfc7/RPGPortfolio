// LJS


#include "Widget/StorageWidget.h"

#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Widget/ItemSlotContainer.h"

UStorageWidget::UStorageWidget()
{
}

void UStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (IsValid(PC))
	{
		StorageSlotContainer->SetInventoryRef(StorageInventoryRef);
	}
}
