// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "InventoryWidget.generated.h"

class UItemSlotContainer;
class UTextBlock;
/**
 * UInventoryWidget
 * 
 * 인벤토리 UI 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UInventoryWidget : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UInventoryWidget();

	//@ 아이템 슬롯 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* ItemSlotContainer;

	//@ 플레이어 골드 표싱
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlayerGold;

	//@ 플레이어 골드 변경 이벤트 핸들러
	UFUNCTION()
	void OnPlayerGoldChanged(int32 NewGoldAmount);
protected:
	virtual void NativeConstruct() override;
};
