// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGStructTypes.h"
#include "ItemSlotMaster.generated.h"

class UDataAsset_RPGItemData;
class UInventoryComponent;
class USizeBox;
class UButton;
class UImage;
class UBorder;
class UTextBlock;
class UOverlay;
/**
 * UItemSlotMaster
 * 
 * 아이템 UI 슬롯 단위 기준 
 */
UCLASS()
class RPGPORTFOLIO_API UItemSlotMaster : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UItemSlotMaster(const FObjectInitializer& ObjectInitializer);

	//@ 슬롯에 아이템이 있는지 여부
	UPROPERTY(BlueprintReadWrite)
	bool SlotHasItem;

	//@ 해당 슬롯의 데이터
	UPROPERTY(BlueprintReadWrite)
	FInventorySlot SlotData;

	//@ 슬롯 데이터 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateSlotData(FInventorySlot UpdateSlotData);

	//@ 위젯 가시성 설정
	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(UWidget* Target, bool IsVisible);

	//@ 인벤토리 참조 설정
	UFUNCTION(BlueprintCallable)
	void SetInventoryRef(UInventoryComponent* InventoryReftoSet);

	//@ 슬롯 크기 설정
	UFUNCTION(BlueprintCallable)
	void SetSlotSizeBox(float Size);

	//@ 인벤토리 소유자와 NPC 타입 확인
	UFUNCTION(BlueprintCallable)
	bool CheckInventoryOwnerAndNPCType();

	//@ 플레이어 인벤토리인지 확인
	UFUNCTION(BlueprintCallable)
	bool CheckIsPlayerInventory();

	//@ 상점 인벤토리인지 확인
	UFUNCTION(BlueprintCallable)
	bool CheckIsStoreInventory();

	//@ 드롭 대상이 상점 인벤토리인지 확인
	UFUNCTION(BlueprintCallable)
	bool CheckDropTargetIsStoreInventory(UItemSlotMaster* TargetSlot);

	//@ 아이템 판매 시도
	UFUNCTION(BlueprintCallable, Category = "Store")
	bool TrySellItem();

	//@ 상점에서 아이템 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnItemButtonClickedInStore();

	//@ 아이템 더블클릭 이벤트 핸들러
	UFUNCTION(BlueprintCallable)
	void OnItemDoubleClicked();

	UInventoryComponent* GetInventoryRef() const { return InventoryRef;}
protected:
	virtual void NativeOnInitialized() override;

	//@ 슬롯 전체 크기 박스
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		USizeBox* RPGSizeBox;

	//@ 슬롯 버튼
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UButton* Button;

	//@ 아이템 아이콘 이미지
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UImage* IconImage;

	//@ 정보 레이어 오버레이
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UOverlay* InfoLayer;

	//@ 수량 레이어 보더
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UBorder* QuantityLayer;

	//@ 수량 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UTextBlock* QuantityText;

	//@ 전략 패턴에 사용할 추가 텍스트블록
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UTextBlock* PlusInfoText;

	//@ 전략 패턴에 사용할 추가 텍스트블록
	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
		UTextBlock* PlusInfoText2;

	//@ 인벤토리 컴포넌트 참조
	UPROPERTY(BlueprintReadWrite)
		UInventoryComponent* InventoryRef;
	
};
