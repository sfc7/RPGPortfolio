// LJS


#include "Widget/ItemPotionHotBar.h"

#include "Character/Player/PlayerCharacter_Fighter.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "GameMode/GameManager/UIManager.h"
#include "Widget/ItemSlotContainer.h"
#include "Component/InventoryComponent.h"

void UItemPotionHotBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (PC)
	{
		UInventoryComponent* PotionInventory = PC->GetPlayerPotionHotBar();

		if (PotionInventory)
		{
			ItemSlotContainer->SetInventoryRef(PotionInventory);
			PotionInventory->OnPotionBarSlotChangedDelegate.AddDynamic(this, &UItemPotionHotBar::OnPotionBarChanged);
		}
	}
}

void UItemPotionHotBar::OnPotionBarChanged()
{
	if (ItemSlotContainer)
	{
		ItemSlotContainer->RefreshSlots();
	}
}
