// LJS


#include "Widget/StoreWidget.h"

#include "Character/NPC/NPC_HumanNPC.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Widget/ItemSlotContainer.h"

UStoreWidget::UStoreWidget()
{
}

void UStoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (PC)
	{
		if (ANPC_HumanNPC* NPC = Cast<ANPC_HumanNPC>(PC->GetInteractTargetActor()))
		{
			StoreSlotContainer->SetInventoryRef(StoreInventoryRef);
		}
	}
}
