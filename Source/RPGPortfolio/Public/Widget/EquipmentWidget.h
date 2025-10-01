// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "EquipmentWidget.generated.h"

class UPlayerEquipmentComponent;
class UOverlay;
class UItemSlotMaster;
/**
 * UStoreWidget
 * 
 * 장비 UI 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UEquipmentWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UEquipmentWidget();

	//@ 장비 슬롯 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* EquipmentSlotContainer;
	
protected:
	virtual void NativeConstruct() override;

	//@ 장비 변경시 슬롯들 재설정
	UFUNCTION()
	void OnEquipmentChanged();
};
