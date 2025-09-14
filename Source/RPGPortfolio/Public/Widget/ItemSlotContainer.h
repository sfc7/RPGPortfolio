// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemSlotContainer.generated.h"

class UScrollBox;
class UItemSlotMaster;
class UInventoryComponent;
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
	void SetInventoryRef(UInventoryComponent* InventoryRefToSet);

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryRef(){ return InventoryRef; }

	UFUNCTION(BlueprintCallable)
	void RefreshSlots();
	
	UFUNCTION(BlueprintCallable)
	void UpdateSlotWidgetData(FInventorySlot UpdateSlot);

protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemSlotWidgetClass;
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInventoryComponent* InventoryRef;
};
