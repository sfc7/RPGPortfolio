// LJS


#include "Widget/ItemSlotMaster.h"

#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Components/Image.h"
#include "GameMode/GameManager/ItemManager.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "Widget/ItemInfomation.h"


UItemSlotMaster::UItemSlotMaster(const FObjectInitializer& ObjectInitializer)
{
	
}

void UItemSlotMaster::UpdateSlotData(FInventorySlot UpdateSlotData)
{
	SlotData = UpdateSlotData;
	SlotHasItem = GetGameInstance()->GetSubsystem<UItemManager>()->IsInventorySlotNotEmpty(SlotData);

	SetWidgetVisibility(InfoLayer, SlotHasItem);

	if (SlotHasItem)
	{
		UDataAsset_RPGItemData* ItemDataAssetObject = SlotData.ItemDataAsset.LoadSynchronous();
		IconImage->SetBrushFromSoftTexture(ItemDataAssetObject->ItemIcon);
		QuantityText->SetText(FText::AsNumber(SlotData.Quantity));

		bool VisibleFlag = ItemDataAssetObject->IsStackable();
		SetWidgetVisibility(QuantityLayer, VisibleFlag);

		UItemInfomation* TooltipWidget = Cast<UItemInfomation>(GetToolTip());
		if (TooltipWidget)
		{
			TooltipWidget->UpdateSlotData(SlotData);
		}	
	}
}

void UItemSlotMaster::SetWidgetVisibility(UWidget* Target, bool IsVisible)
{
	if (IsVisible) 
	{
		Target->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Target->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlotMaster::SetInventoryRef(UPlayerInventoryComponent* InventoryReftoSet)
{
	InventoryRef = InventoryReftoSet;
}

void UItemSlotMaster::SetSlotSizeBox(float Size)
{
	RPGSizeBox->SetWidthOverride(Size);
	RPGSizeBox->SetHeightOverride(Size);
}
