// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "EquipmentWidget.generated.h"

class UPlayerEquipmentComponent;
class UOverlay;
class UItemSlotMaster;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UEquipmentWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UEquipmentWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* EquipmentSlotContainer;
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnEquipmentChanged();
};
