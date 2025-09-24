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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UStoreWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UStoreWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* StoreSlotContainer;

	UPROPERTY(BlueprintReadWrite)
	UInventoryComponent*  StoreInventoryRef;

	UFUNCTION(BlueprintCallable)
	void OnStoreItemPurchase(UItemSlotMaster* ClickedSlot);

	UFUNCTION(BlueprintCallable)
	void OnSellItem(UItemSlotMaster* ClickedSlot);

	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* PurchaseConfirmOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PurchaseConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PurchaseCancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* SellConfirmOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SellConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SellCancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PurchaseItemText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SellItemText;

private:
	UFUNCTION()
	void OnPurchaseConfirmClicked();

	UFUNCTION()
	void OnPurchaseCancelClicked();

	UFUNCTION()
	void OnSellConfirmClicked();

	UFUNCTION()
	void OnSellCancelClicked();

	UPROPERTY()
	UItemSlotMaster* CurrentSelectedItem;
};
