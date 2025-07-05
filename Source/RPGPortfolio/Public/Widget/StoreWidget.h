// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/InventoryWidget.h"
#include "StoreWidget.generated.h"

class UPlayerInventoryComponent;
class UItemSlotContainer;
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

protected:
	virtual void NativeConstruct() override;
};
