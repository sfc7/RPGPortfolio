// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

struct FInventorySlot;
class UDataAsset_RPGItemData;
class UInventoryComponent;
/**
 * UItemManager
 * 
 * 아이템 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UItemManager();

	//@ 모든 인벤토리 컴포넌트에 대해서 인벤토리 슬롯이 비어있는지 확인가능한 함수
	UFUNCTION(BlueprintPure)
	bool IsInventorySlotEmpty(FInventorySlot InventorySlot);

	//@ 모든 인벤토리 컴포넌트에 대해서 인벤토리 슬롯이 비어있지 않은지 확인가능한 함수
	UFUNCTION(BlueprintPure)
	bool IsInventorySlotNotEmpty(FInventorySlot InventorySlot);

	//@ 아이템을 InventorySlot 단위로 데이터를 만듬
	UFUNCTION(BlueprintPure)
	FInventorySlot MakeItemToAdd(TSoftObjectPtr<UDataAsset_RPGItemData> ItemToAdd, int32 Quantity);

	//@ 모든 인벤토리 컴포넌트에 대해서 인벤토리 슬롯 인덱스 설정
	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(FInventorySlot InventorySlottoSet, int32 Index);

	//@ 스택 가능하고 동일 타입이며 공간이 있는지 확인 
	UFUNCTION(BlueprintPure)
	bool IsStackableAndIsEqualAndHaveSpace(FInventorySlot& TargetSlot, FInventorySlot& SlotToStack);

	//@ UI에서 인벤토리 슬롯 드롭 처리
	UFUNCTION(BlueprintCallable)
	void OnInventorySlotDrop(UInventoryComponent* FromContainerInventoryComponent, UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex) const;

	//@FInventorySlot에서 스택 크기 가져오기 
	UFUNCTION(BlueprintCallable)
	int32 GetStackSize(FInventorySlot TargetSlot);

	//@ 인벤토리 컴포넌트가 Store인지 확인
	UFUNCTION(BlueprintCallable)
	bool CheckIfPlayerToStore(UInventoryComponent* FromContainerInventoryComponent, UInventoryComponent* ToInventoryComponent) const;

	//@ 인벤토리 컴포넌트가 Equipment인지 확인
	UFUNCTION(BlueprintCallable)
	bool CheckIfToEquipment(UInventoryComponent* FromContainerInventoryComponent) const;

	//@ UI에서 아이템 들어서 상점에 드롭할때의 함수
	UFUNCTION(BlueprintCallable)
	void HandlePlayerToStoreDrop(UInventoryComponent* FromContainerInventoryComponent, int32 FromIndex) const;

	//@ UI에서 장비 아이템 들어서 드롭할때의 함수
	UFUNCTION(BlueprintCallable)
	void HandleEquipmentDrop(UInventoryComponent* FromContainerInventoryComponent, UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex) const;
	
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
};
