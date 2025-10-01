// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "StorageWidget.generated.h"

class UInventoryComponent;
/**
 * UStoreWidget
 * 
 * 창고 UI 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UStorageWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UStorageWidget();

	//@ 창고 슬롯 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* StorageSlotContainer;

	//@ 창고 인벤토리 참조
	UPROPERTY(BlueprintReadWrite)
	UInventoryComponent*  StorageInventoryRef;

protected:
	virtual void NativeConstruct() override;
};
