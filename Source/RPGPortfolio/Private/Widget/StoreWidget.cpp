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

	if (SellConfirmButton)
	{
		SellConfirmButton->OnClicked.AddDynamic(this, &UStoreWidget::OnSellConfirmClicked);
	}	

	if (SellCancelButton)
	{
		SellCancelButton->OnClicked.AddDynamic(this, &UStoreWidget::OnSellCancelClicked);
	}

	if (SellConfirmOverlay)
	{
		SellConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UStoreWidget::OnStoreItemPurchase(UItemSlotMaster* ClickedSlot)
{
	if (!IsValid(ClickedSlot))
	{
		return;
	}

	if (!IsValid(PurchaseConfirmOverlay))
	{
		return;
	}

	if (!IsValid(PurchaseItemText))
	{
		return;
	}
	
	CurrentSelectedItem = ClickedSlot;
	UDataAsset_RPGItemData* ItemData = ClickedSlot->SlotData.ItemDataAsset.LoadSynchronous();
	
	FText CombinedText = FText::Format(
		FText::FromString(TEXT("{0}\n가격: {1} 골드")), 
		ItemData->ItemName, FText::AsNumber(ItemData->GoldValue));
	PurchaseItemText->SetText(CombinedText);
			
	PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UStoreWidget::OnPurchaseConfirmClicked()
{
	if (CurrentSelectedItem)
	{
		APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
		if (Player && Player->GetPlayerInventoryComponent())
		{
			bool bSuccess = Player->GetPlayerInventoryComponent()->TryPurchaseFromStore(
				CurrentSelectedItem->SlotData, 
				CurrentSelectedItem->GetInventoryRef()
			);

			// OnPlayerGoldChanged(Player->GetPlayerInventoryComponent()->GetPlayerGold());
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

void UStoreWidget::OnSellItem(UItemSlotMaster* ClickedSlot)
{
	if (!IsValid(ClickedSlot))
	{
		return;
	}

	if (!IsValid(SellConfirmOverlay))
	{
		return;
	}

	if (!IsValid(SellItemText))
	{
		return;
	}
	
	CurrentSelectedItem = ClickedSlot;
	UDataAsset_RPGItemData* ItemData = ClickedSlot->SlotData.ItemDataAsset.LoadSynchronous();
	
	FText CombinedText = FText::Format(
		FText::FromString(TEXT("{0}\n가격: {1} 골드")), 
		ItemData->ItemName, FText::AsNumber(ItemData->GoldValue));
	SellItemText->SetText(CombinedText);

	SellConfirmOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UStoreWidget::OnSellConfirmClicked()
{
	if (!IsValid(CurrentSelectedItem))
	{
		return;
	}
	
	if (!IsValid(SellConfirmOverlay))
	{
		return;
	}
	
	const bool bSuccess = CurrentSelectedItem->TrySellItem();
       
	if (bSuccess)
	{
		APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
		if (Player && Player->GetPlayerInventoryComponent())
			OnPlayerGoldChanged(Player->GetPlayerInventoryComponent()->GetPlayerGold());
	}
	
	SellConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
    
	CurrentSelectedItem = nullptr;
}

void UStoreWidget::OnSellCancelClicked()
{
	if (!IsValid(SellConfirmOverlay))
	{
		return;
	}
	
	SellConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);

	CurrentSelectedItem = nullptr;
}