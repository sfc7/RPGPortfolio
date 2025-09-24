// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

// 인벤토리 UI에서 선택된 아이템 타입 
UENUM(BlueprintType)
enum class EInventoryTypeStrategy : uint8
{
	Default,
	Equipment,
	Potion,
	Material,
	None
};

// 인벤토리가 처한 상황 ex) 상점이 켜진 상태에서의 인벤토리, 장비창이 켜진 상태에서의 인벤토리
UENUM(BlueprintType)
enum class EInventorySituationStrategy : uint8
{

	Default,
	InOpenStore,
	InOpenStorage,
	InOpenEquipment,
	None
};

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
public:	
	UPlayerInventoryComponent();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> MaterialItemSlots;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> PotionItemSlots;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> EquipmentItemSlots;
	
	virtual void SetupSlots(int32 SlotAmountstoSetup) override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentInventoryTypeStrategy(EInventoryTypeStrategy InventoryTypeStrategyToSet);

	UFUNCTION(BlueprintCallable)
	void SetCurrentInventorySituationStrategy(EInventorySituationStrategy InventorySituationStrategyToSet);
	
protected:
	virtual void BeginPlay() override;
};

// 장비 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UEquipmentTypeStrategy : public UObject, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};

// 포션 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UPotionTypeStrategy : public UInventoryTypeStrategy, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};

// 재료 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UMaterialTypeStrategy : public UInventoryTypeStrategy, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};

// 상점창이 열린 상태의 인벤토리 전략
UCLASS()
class RPGPORTFOLIO_API UInOpenStoreStrategy : public UObject, public IInventorySituationStrategy
{
	GENERATED_BODY()

public:
	virtual void HandleItemDoubleClick(UInventoryComponent* Inventory, FInventorySlot& SlotData) override;
	virtual void HandleItemRightClick(UInventoryComponent* Inventory, FInventorySlot& SlotData) override;
};

// 장비창이 열린 상태의 인벤토리
UCLASS()
class RPGPORTFOLIO_API UInOpenEquipmentStrategy : public UObject, public IInventorySituationStrategy
{
	GENERATED_BODY()

public:
	virtual void HandleItemDoubleClick(UInventoryComponent* Inventory, FInventorySlot& SlotData) override;
	virtual void HandleItemRightClick(UInventoryComponent* Inventory, FInventorySlot& SlotData) override;
};
