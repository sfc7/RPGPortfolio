// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "StoreWidget.generated.h"

class UInventoryComponent;
class UItemSlotContainer;
class UItemSlotMaster;
class UButton;
class UOverlay;
class UTextBlock;

/**
 * UStoreWidget
 * 
 * 상점 UI 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UStoreWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UStoreWidget();

	//@ 상점 슬롯 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* StoreSlotContainer;

	//@ 상점 인벤토리 참조
	UPROPERTY(BlueprintReadWrite)
	UInventoryComponent*  StoreInventoryRef;

	//@ 상점 아이템 구매 처리
	UFUNCTION(BlueprintCallable)
	void OnStoreItemPurchase(UItemSlotMaster* ClickedSlot);

	//@ 아이템 판매 처리
	UFUNCTION(BlueprintCallable)
	void OnSellItem(UItemSlotMaster* ClickedSlot);

protected:
	virtual void NativeConstruct() override;

	//@ 구매 확인 오버레이
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* PurchaseConfirmOverlay;

	//@ 구매 확인 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PurchaseConfirmButton;

	//@ 구매 취소 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PurchaseCancelButton;

	//@ 판매 확인 오버레이
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* SellConfirmOverlay;

	//@ 판매 확인 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SellConfirmButton;

	//@ 판매 취소 버튼
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SellCancelButton;

	//@ 구매 아이템 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PurchaseItemText;
	
	//@ 판매 아이템 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SellItemText;

private:
	//@ 구매 확인 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnPurchaseConfirmClicked();

	//@ 구매 취소 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnPurchaseCancelClicked();

	//@ 판매 확인 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnSellConfirmClicked();

	//@ 판매 취소 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnSellCancelClicked();

	//@ 현재 마우스로 선택된 아이템 슬롯
	UPROPERTY()
	UItemSlotMaster* CurrentSelectedItem;
};
