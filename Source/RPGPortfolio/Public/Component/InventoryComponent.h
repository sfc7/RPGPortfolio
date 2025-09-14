// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGStructTypes.h"
#include "Item/InventoryTypeStrategy.h"
#include "InventoryComponent.generated.h"

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
class UInventoryStrategy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChangedDelegate, FInventorySlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChangedDelegate, int32, GoldAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPotionBarSlotChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentSlotChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintPure)
	int32 GetPlayerGold() const { return Gold; }
	
	UFUNCTION(BlueprintCallable)
	virtual void SetupSlots(int32 SlotAmountstoSetup);

	UFUNCTION(BlueprintCallable)
	bool AddItem(FInventorySlot ItemToAdd);

	UFUNCTION(BlueprintCallable)
	bool AddItemToIndex(FInventorySlot ItemToAdd, int32 ToIndex, bool& OutAreAllItemAdded);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemToIndex(int32 ToIndex);

	UFUNCTION(BlueprintCallable)
	void SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet);

	UFUNCTION(BlueprintCallable)
	void SetGold(int32 GoldAmount);
	
	UFUNCTION(BlueprintCallable)
	bool FindEmptySlot(FInventorySlot& OutEmptySlot);
	
	FOnInventorySlotChangedDelegate OnInventorySlotChangedDelegate;
	FOnGoldChangedDelegate OnGoldChangedDelegate;
	FOnPotionBarSlotChangedDelegate OnPotionBarSlotChangedDelegate;
	FOnEquipmentSlotChangedDelegate OnEquipmentSlotChangedDelegate;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	int32 SlotAmounts;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> DefaultItemSlots;

	UPROPERTY(BlueprintReadWrite)
	UItemManager* ItemManager;
	
	UFUNCTION(BlueprintPure)
	FInventorySlot SetQuantityAtSlot(FInventorySlot& TargetSlot, int32 QuantityToSet);

	UFUNCTION(BlueprintCallable)
	bool TransferItem(UInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintPure)
	bool IsValidSlotIndex(int32 FindIndex);

	UFUNCTION(BlueprintCallable)
	bool StackItemOnTransfer(FInventorySlot TargetSlot, FInventorySlot FromSlot, bool& OutAreAllItemAdded);

	UFUNCTION(BlueprintCallable)
	void SwapIndex(FInventorySlot TargetSlot, FInventorySlot FromSlot);

	UFUNCTION(BlueprintCallable)
	void EquipItem(FInventorySlot FromSlot);

	UFUNCTION(BlueprintCallable)
	void UnEquipItem(FInventorySlot FromSlot);

	UFUNCTION(BlueprintCallable)
	EInventoryType GetInventoryType() const { return InventoryType;}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<IInventoryStrategy> CurrentInventoryStrategy;
	
	TArray<FInventorySlot>& GetCurrentItemSlots();
	
	const TArray<FInventorySlot>& GetCurrentItemSlots() const;

	void SetDefaultInventoryStrategy();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EInventoryType InventoryType = EInventoryType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Gold = 0;

	UPlayerEquipmentComponent* EquipmentComponentRef;
};



// 기본 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UDefaultInventoryStrategy : public UObject, public IInventoryStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};

