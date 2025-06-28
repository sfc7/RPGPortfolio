// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemSlotMaster.generated.h"

class UDataAsset_RPGItemData;
class UPlayerInventoryComponent;
class USizeBox;
class UButton;
class UImage;
class UBorder;
class UTextBlock;
class UOverlay;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItemSlotMaster : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UItemSlotMaster(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite)
	bool SlotHasItem;

	UPROPERTY(BlueprintReadWrite)
	FInventorySlot SlotData;

	UFUNCTION(BlueprintCallable)
	void UpdateSlotData(FInventorySlot UpdateSlotData);
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(UWidget* Target, bool IsVisible);

	UFUNCTION(BlueprintCallable)
	void SetInventoryRef(UPlayerInventoryComponent* InventoryReftoSet);

	UFUNCTION(BlueprintCallable)
	void SetSlotSizeBox(float Size);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		USizeBox* RPGSizeBox;
	
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UButton* Button;
	
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UImage* IconImage;
	
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UOverlay* InfoLayer;
	
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UBorder* QuantityLayer;
	
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UTextBlock* QuantityText;
	
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UTextBlock* WeightText;
	
	UPROPERTY(BlueprintReadWrite)
		UPlayerInventoryComponent* InventoryRef;
	
};
