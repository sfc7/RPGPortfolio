// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemSlotContainer.generated.h"

class UScrollBox;
class UItemSlotMaster;
class UInventoryComponent;
class UUniformGridPanel;
/**
 * UItemSlotContainer
 * 
 * 아이템 슬롯 컨테이너,아이템 슬롯 그리드 관리
 */

UCLASS()
class RPGPORTFOLIO_API UItemSlotContainer : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UItemSlotContainer(const FObjectInitializer& ObjectInitializer);

	//@ 인벤토리 슬롯 스크롤  
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	//@ 그리드 패널
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* GridForSlots;

	//@ 총 슬롯 개수
	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	int32 TotalSlots;

	//@ 행당 슬롯 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotsPerRow;

	//@ 슬롯 위젯 배열
	UPROPERTY(BlueprintReadWrite)
	TArray<UItemSlotMaster*> SlotWidgets;

	//@ 슬롯 크기 조정
	UFUNCTION(BlueprintCallable)
	void ResizeSlot();

	//@ 인벤토리 참조 설정
	UFUNCTION(BlueprintCallable)
	void SetInventoryRef(UInventoryComponent* InventoryRefToSet);

	//@ 인벤토리 참조 가져오기
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryRef(){ return InventoryRef; }

	//@ 슬롯 새로고침
	UFUNCTION(BlueprintCallable)
	void RefreshSlots();

	//@ 슬롯 위젯 데이터 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateSlotWidgetData(FInventorySlot UpdateSlot);

protected:
	virtual void NativeOnInitialized() override;

	//@ 아이템 슬롯 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemSlotWidgetClass;
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInventoryComponent* InventoryRef;
};
