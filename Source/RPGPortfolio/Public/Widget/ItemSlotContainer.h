// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemSlotContainer.generated.h"

class UScrollBox;
class UItemSlotMaster;
class UPlayerInventoryComponent;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItemSlotContainer : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UItemSlotContainer(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* GridForSlots;
	
	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	int32 TotalSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotsPerRow;

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemSlotMaster*> SlotWidgets;

	UFUNCTION(BlueprintCallable)
	void ResizeSlot();

	UFUNCTION(BlueprintCallable)
	void SetInventoryRef(UPlayerInventoryComponent* InventoryRefToSet);

	UFUNCTION(BlueprintCallable)
	void RefreshSlots();
	
	UFUNCTION(BlueprintCallable)
	void UpdateSlotWidgetData(FInventorySlot UpdateSlot);

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPlayerInventoryComponent* InventoryRef;
};
