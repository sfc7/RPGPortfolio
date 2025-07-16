// LJS


#include "Widget/StoreWidget.h"

#include "Character/NPC/NPC_HumanNPC.h"
#include "Character/Player/PlayerCharacter_Fighter.h"
#include "Widget/ItemSlotContainer.h"
#include "Widget/ItemSlotMaster.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "Components/TextBlock.h"
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

	if (PurchaseConfirmButton)
	{
		PurchaseConfirmButton->OnClicked.AddDynamic(this, &UStoreWidget::OnPurchaseConfirmClicked);
	}	

	if (PurchaseCancelButton)
	{
		PurchaseCancelButton->OnClicked.AddDynamic(this, &UStoreWidget::OnPurchaseCancelClicked);
	}

	if (PurchaseConfirmOverlay)
	{
		PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UStoreWidget::	OnStoreItemPurchase(UItemSlotMaster* ClickedSlot)
{
	if (!ClickedSlot || !ClickedSlot->CanPurchaseItem())
	{
		return;
	}
	
	CurrentSelectedItem = ClickedSlot;
	UDataAsset_RPGItemData* ItemData = ClickedSlot->SlotData.ItemDataAsset.LoadSynchronous();
	
	if (ItemText)
		if (ItemText)
		{
			FText CombinedText = FText::Format(
				FText::FromString(TEXT("{0}\n가격: {1} 골드")), 
				ItemData->ItemName, 
				FText::AsNumber(ItemData->GoldValue)
			);
			ItemText->SetText(CombinedText);
		}

	if (PurchaseConfirmOverlay)
	{
		PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void UStoreWidget::OnPurchaseConfirmClicked()
{
	if (CurrentSelectedItem)
	{
		bool bSuccess = CurrentSelectedItem->TryPurchaseItem();
		if (bSuccess)
		{
			APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
			if (Player && Player->GetPlayerInventoryComponent())
				OnPlayerGoldChanged(Player->GetPlayerInventoryComponent()->GetPlayerGold());
		}
	}

	if (PurchaseConfirmOverlay)
		PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
	CurrentSelectedItem = nullptr;
}

void UStoreWidget::OnPurchaseCancelClicked()
{
	if (PurchaseConfirmOverlay)
		PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
	CurrentSelectedItem = nullptr;
}
