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
	if (IsValid(PC))
	{
		UInventoryComponent* PotionInventory = PC->GetPlayerPotionHotBar();

		if (IsValid(PotionInventory))
		{
			ItemSlotContainer->SetInventoryRef(PotionInventory);
			PotionInventory->OnPotionBarSlotChangedDelegate.AddDynamic(this, &UItemPotionHotBar::OnPotionBarChanged);
		}
	}
}

void UItemPotionHotBar::OnPotionBarChanged()
{
	//@ 포션 퀵슬롯 변경시 슬롯들 재설정
	if (IsValid(ItemSlotContainer))
	{
		ItemSlotContainer->RefreshSlots();
	}
}
