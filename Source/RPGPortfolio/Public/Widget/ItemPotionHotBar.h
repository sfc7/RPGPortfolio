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
	UItemPotionHotBar(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* ItemSlotContainer;
};
