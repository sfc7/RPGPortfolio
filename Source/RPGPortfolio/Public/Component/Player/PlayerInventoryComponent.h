// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGStructTypes.h" 
#include "PlayerInventoryComponent.generated.h"

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
class UPlayerInventoryComponent;
class UDataAsset_RPGItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChangedDelegate, FInventorySlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChangedDelegate, int32, GoldAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPotionBarSlotChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInventoryComponent();

	UFUNCTION(BlueprintPure)
	int32 GetPlayerGold() const { return PlayerGold; }
	
	UFUNCTION(BlueprintCallable)
	void SetupSlots(int32 SlotAmountstoSetup);

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
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	int32 SlotAmounts;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> ItemSlots;

	UPROPERTY(BlueprintReadWrite)
	UItemManager* ItemManager;
	
	UFUNCTION(BlueprintPure)
	FInventorySlot SetQuantityAtSlot(FInventorySlot& TargetSlot, int32 QuantityToSet);

	UFUNCTION(BlueprintCallable)
	bool TransferItem(UPlayerInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintPure)
	bool IsValidSlotIndex(int32 FindIndex);

	UFUNCTION(BlueprintCallable)
	bool StackItemOnTransfer(FInventorySlot TargetSlot, FInventorySlot FromSlot, bool& OutAreAllItemAdded);

	UFUNCTION(BlueprintCallable)
	void SwapIndex(FInventorySlot TargetSlot, FInventorySlot FromSlot);

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EInventoryType InventoryType = EInventoryType::None;
	
	UUserWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 PlayerGold = 0;
};
