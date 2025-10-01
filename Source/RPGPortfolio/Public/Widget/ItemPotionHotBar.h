// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "ItemPotionHotBar.generated.h"

class UItemSlotContainer;
/**
 * UStoreWidget
 * 
 * 포션 퀵슬롯 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UItemPotionHotBar : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 포션 퀵슬롯 컨테이너
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UItemSlotContainer* ItemSlotContainer;

protected:
	virtual void NativeOnInitialized() override;

	//@ 포션 퀵슬롯 변경시 슬롯들 재설정
	UFUNCTION()
	void OnPotionBarChanged();
	
};
