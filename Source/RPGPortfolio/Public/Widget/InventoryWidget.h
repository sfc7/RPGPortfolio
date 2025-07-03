// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "InventoryWidget.generated.h"

class UItemSlotContainer;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UInventoryWidget : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UInventoryWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* ItemSlotContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlayerGold;
protected:
	virtual void NativeConstruct() override;
};
