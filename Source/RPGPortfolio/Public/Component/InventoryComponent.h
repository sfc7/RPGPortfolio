// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGStructTypes.h"
#include "Item/InventoryTypeStrategy.h"
#include "Item/InventorySituationStrategy.h"
#include "InventoryComponent.generated.h"

//@ 인벤토리 타입 - 각 인벤토리의 용도 구분
UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	PlayerInventory,
	Storage,
	Potion,
	Equipment,
	None
};

class UItemManager;
class UInventoryComponent;
class UDataAsset_RPGItemData;
class UInventoryTypeStrategy;
class UInventorySituationStrategy;

//@ 인벤토리 슬롯 변경 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChangedDelegate, FInventorySlot, Slot);
//@ 골드 변경 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChangedDelegate, int32, GoldAmount);
//@ 포션 바 슬롯 변경 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPotionBarSlotChangedDelegate);
//@ 장비 슬롯 변경 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentSlotChangedDelegate);

/**
 * UInventoryComponent
 * 
 * 인벤토리에 사용할 컴포넌트의 Base
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	//@ 현재 인벤토리 UI 타입 전략 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<IInventoryTypeStrategy> CurrentInventoryTypeStrategy;
	
	//@ 현재 인벤토리 처한 상황 전략
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<IInventorySituationStrategy> CurrentInventorySituationStrategy;

	//@ 기본 인벤토리 UI 타입 전략 설정
	void SetDefaultInventoryTypeStrategy();

	//@ 기본 인벤토리 처한 상황 전략 설정
	void SetDefaultInventorySituationStrategy();
	
	UFUNCTION(BlueprintPure)
	int32 GetPlayerGold() const { return Gold; }

	UFUNCTION(BlueprintCallable)
	void SetGold(int32 GoldAmount);

	//@ 인벤토리 슬롯 개수 설정
	UFUNCTION(BlueprintCallable)
	virtual void SetupSlots(int32 SlotAmountstoSetup);

	//@ 아이템 추가 (자동으로 빈 슬롯 찾아서 추가)
	UFUNCTION(BlueprintCallable)
	bool AddItem(FInventorySlot ItemToAdd);

	//@ 특정 인덱스에 아이템 추가
	UFUNCTION(BlueprintCallable)
	bool AddItemToIndex(FInventorySlot ItemToAdd, int32 ToIndex, bool& OutAreAllItemAdded);

	//@ 특정 인덱스의 아이템 제거
	UFUNCTION(BlueprintCallable)
	bool RemoveItemToIndex(int32 ToIndex);

	//@ 아이템 설정 (슬롯에 직접 아이템 배치)
	UFUNCTION(BlueprintCallable)
	void SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet);

	//@ 인벤토리 내 빈 슬롯 찾기 
	UFUNCTION(BlueprintCallable)
	bool FindEmptySlot(FInventorySlot& OutEmptySlot);

	//@ 델리게이트
	FOnInventorySlotChangedDelegate OnInventorySlotChangedDelegate;
	FOnGoldChangedDelegate OnGoldChangedDelegate;
	FOnPotionBarSlotChangedDelegate OnPotionBarSlotChangedDelegate;
	FOnEquipmentSlotChangedDelegate OnEquipmentSlotChangedDelegate;

	//@ 인벤토리 슬롯 개수
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	int32 SlotAmounts;

	//@ 기본타입 아이템 슬롯 배열
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> DefaultItemSlots;

	//@ 슬롯 인덱스 유효성 검사
	UFUNCTION(BlueprintPure)
	bool IsValidSlotIndex(int32 FindIndex);
	
	//@ 슬롯에 있는 아이템 수량 설정
	UFUNCTION(BlueprintPure)
	FInventorySlot SetQuantityAtSlot(FInventorySlot& TargetSlot, int32 QuantityToSet);

	//@ 인벤토리간 아이템 이동
	UFUNCTION(BlueprintCallable)
	bool TransferItem(UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex);

	//@ 인벤토리간 스택 아이템 이동 
	UFUNCTION(BlueprintCallable)
	bool StackItemOnTransfer(FInventorySlot TargetSlot, FInventorySlot FromSlot, bool& OutAreAllItemAdded);

	//@ 마우스 액션으로 슬롯 위치 교체시 
	UFUNCTION(BlueprintCallable)
	void SwapIndex(FInventorySlot TargetSlot, FInventorySlot FromSlot);

	//@ 마우스 액션으로 아이템 장착
	UFUNCTION(BlueprintCallable)
	void EquipItem(FInventorySlot FromSlot);

	//@ 마우스 액션으로 아이템 해제
	UFUNCTION(BlueprintCallable)
	void UnEquipItem(FInventorySlot FromSlot);

	UFUNCTION(BlueprintCallable)
	EInventoryType GetInventoryType() const { return InventoryType;}

	//@ 현재 아이템 인벤토리 슬롯 가져오기 (InventoryType 전략 패턴 시)
	TArray<FInventorySlot>& GetCurrentItemSlots();
	
	const TArray<FInventorySlot>& GetCurrentItemSlots() const;

	//@ 슬롯 더블클릭 처리 (InventorySituation 전략 패턴 시)
	void HandleSlotDoubleClick(FInventorySlot& SlotData);

	//@ 슬롯 우클릭 처리 (InventorySituation 전략 패턴 시)
	void HandleSlotRightClick(FInventorySlot& SlotData);

	//@ 아이템 판매 시도
	UFUNCTION(BlueprintCallable, Category = "Store")
	bool TrySellItem(const FInventorySlot& SlotToSell);

	//@ 상점에서 아이템 구매 시도
	UFUNCTION(BlueprintCallable, Category = "Store")
	bool TryPurchaseFromStore(const FInventorySlot& StoreSlot, UInventoryComponent* StoreInventory);

	//@ 상점에서 아이템 구매 가능 확인
	UFUNCTION(BlueprintCallable, Category = "Store")
	bool CanPurchaseFromStore(const FInventorySlot& StoreSlot) const;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EInventoryType InventoryType = EInventoryType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Gold = 0;

	UPlayerEquipmentComponent* EquipmentComponentRef;

	UPROPERTY(BlueprintReadWrite)
	UItemManager* ItemManager;
};



// 기본 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UDefaultTypeStrategy : public UObject, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};


UCLASS()
class RPGPORTFOLIO_API UDefaultSituationStrategy : public UObject, public IInventorySituationStrategy
{
	GENERATED_BODY()

public:
	virtual void HandleItemDoubleClick(UInventoryComponent* Inventory, FInventorySlot& SlotData) override;
	virtual void HandleItemRightClick(UInventoryComponent* Inventory, FInventorySlot& SlotData) override;
};