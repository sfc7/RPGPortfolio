// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "StoreWidget.generated.h"

class UPlayerInventoryComponent;
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
	UPlayerInventoryComponent*  StoreInventoryRef;

	UFUNCTION(BlueprintCallable)
	void OnStoreItemPurchase(UItemSlotMaster* ClickedSlot);

	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* PurchaseConfirmOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PurchaseConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PurchaseCancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemText;

private:
	UFUNCTION()
	void OnPurchaseConfirmClicked();

	UFUNCTION()
	void OnPurchaseCancelClicked();

	UPROPERTY()
	UItemSlotMaster* CurrentSelectedItem;
};
