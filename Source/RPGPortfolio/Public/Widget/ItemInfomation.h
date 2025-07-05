// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemInfomation.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItemInfomation : public URPGWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlotData(FInventorySlot UpdateSlotData);

	UFUNCTION(BlueprintCallable)
	FText GetItemDescription(TSoftObjectPtr<UDataAsset_RPGItemData> ItemData);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemDescription;
};
