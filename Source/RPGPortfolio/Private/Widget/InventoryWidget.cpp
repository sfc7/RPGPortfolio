// LJS


#include "Widget/InventoryWidget.h"
#include "Component/InventoryComponent.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Widget/ItemSlotContainer.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "Components/TextBlock.h"

UInventoryWidget::UInventoryWidget()
{
}

void UInventoryWidget::OnPlayerGoldChanged(int32 NewGoldAmount)
{
	if (PlayerGold)
	{
		PlayerGold->SetText(FText::AsNumber(NewGoldAmount));
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (PC)
	{
		ItemSlotContainer->SetInventoryRef(	PC->GetPlayerInventoryComponent());
		OnPlayerGoldChanged(PC->GetPlayerInventoryComponent()->GetPlayerGold());
	}

	ItemSlotContainer->GetInventoryRef()->OnGoldChangedDelegate.AddDynamic(this, &UInventoryWidget::OnPlayerGoldChanged);
}
