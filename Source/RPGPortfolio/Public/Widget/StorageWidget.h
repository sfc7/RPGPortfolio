// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "StorageWidget.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UStorageWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UStorageWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* StorageSlotContainer;

	UPROPERTY(BlueprintReadWrite)
	UInventoryComponent*  StorageInventoryRef;


protected:
	virtual void NativeConstruct() override;
};
