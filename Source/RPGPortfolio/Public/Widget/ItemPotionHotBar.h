// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "ItemPotionHotBar.generated.h"

class UItemSlotContainer;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItemPotionHotBar : public URPGWidgetBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* ItemSlotContainer;

protected:
	virtual void NativeOnInitialized() override;
	
};
