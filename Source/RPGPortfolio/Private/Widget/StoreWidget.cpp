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

	const APlayerCharacter_Fighter* PC = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
	if (!IsValid(PC)) return;

	// NPC에서 상점 인벤토리 설정
	const ANPC_HumanNPC* NPC = Cast<ANPC_HumanNPC>(PC->GetInteractTargetActor());
	if (IsValid(NPC) && IsValid(StoreSlotContainer))
	{
		StoreSlotContainer->SetInventoryRef(StoreInventoryRef);
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
	if (!IsValid(ClickedSlot)) return;

	if (!IsValid(PurchaseConfirmOverlay)) return;
	
	if (!IsValid(PurchaseItemText)) return;

	// 현재 선택된 아이템 설정
	CurrentSelectedItem = ClickedSlot;
	UDataAsset_RPGItemData* ItemData = ClickedSlot->SlotData.ItemDataAsset.LoadSynchronous();

	// 구매 확인 텍스트 설정
	FText CombinedText = FText::Format(
		FText::FromString(TEXT("{0}\n가격: {1} 골드")), 
		ItemData->ItemName, FText::AsNumber(ItemData->GoldValue));
	PurchaseItemText->SetText(CombinedText);
			
	PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UStoreWidget::OnPurchaseConfirmClicked()
{
	// 현재 마우스에 선택된 아이템이 있으면
	if (CurrentSelectedItem)
	{
		APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
		if (Player && Player->GetPlayerInventoryComponent())
		{
			
			// 구매 시도
			bool bSuccess = Player->GetPlayerInventoryComponent()->TryPurchaseFromStore(
				CurrentSelectedItem->SlotData, 
				CurrentSelectedItem->GetInventoryRef()
			);
		}
	}

	// 구매 이후 처리
	if (PurchaseConfirmOverlay)
		PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
    
	CurrentSelectedItem = nullptr;
}

void UStoreWidget::OnPurchaseCancelClicked()
{
	// 취소 이후 처리
	if (PurchaseConfirmOverlay)
		PurchaseConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);

	CurrentSelectedItem = nullptr;
}

void UStoreWidget::OnSellItem(UItemSlotMaster* ClickedSlot)
{
	if (!IsValid(ClickedSlot)) return;
	
	if (!IsValid(SellConfirmOverlay)) return;
	
	if (!IsValid(SellItemText)) return;

	// 현재 선택된 아이템 설정
	CurrentSelectedItem = ClickedSlot;
	UDataAsset_RPGItemData* ItemData = ClickedSlot->SlotData.ItemDataAsset.LoadSynchronous();

	// 판매 확인 텍스트 설정
	FText CombinedText = FText::Format(
		FText::FromString(TEXT("{0}\n가격: {1} 골드")), 
		ItemData->ItemName, FText::AsNumber(ItemData->GoldValue));
	SellItemText->SetText(CombinedText);

	SellConfirmOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UStoreWidget::OnSellConfirmClicked()
{
	if (!IsValid(CurrentSelectedItem)) return;
	
	if (!IsValid(SellConfirmOverlay)) return;

	// 판매 시도 확인
	const bool bSuccess = CurrentSelectedItem->TrySellItem();

	// 판매 시도 
	if (bSuccess)
	{
		APlayerCharacter_Fighter* Player = Cast<APlayerCharacter_Fighter>(GetOwningPlayerPawn());
		if (IsValid(Player) && Player->GetPlayerInventoryComponent())
			OnPlayerGoldChanged(Player->GetPlayerInventoryComponent()->GetPlayerGold());
	}

	// 판매 이후 처리
	SellConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);
    
	CurrentSelectedItem = nullptr;
}

void UStoreWidget::OnSellCancelClicked()
{
	if (!IsValid(SellConfirmOverlay)) return;
	
	// 판매 이후 처리
	SellConfirmOverlay->SetVisibility(ESlateVisibility::Hidden);

	CurrentSelectedItem = nullptr;
}