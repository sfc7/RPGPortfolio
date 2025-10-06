// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemInfomation.generated.h"

class UImage;
class UTextBlock;
/**
 * UItemInfomation
 * 
 * 아이템 정보 툴팁 
 */
UCLASS()
class RPGPORTFOLIO_API UItemInfomation : public URPGWidgetBase
{
	GENERATED_BODY()
	
public:
	//@ 슬롯 데이터 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateSlotData(FInventorySlot UpdateSlotData);

	//@ 아이템 설명 텍스트 생성
	UFUNCTION(BlueprintCallable)
	FText GetItemDescription(TSoftObjectPtr<UDataAsset_RPGItemData> ItemData);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	//@ 아이템 아이콘 이미지
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	//@ 아이템 이름 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemName;

	//@ 아이템 설명 텍스트
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemDescription;
};
