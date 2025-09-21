// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EInventoryTypeStrategy : uint8
{
	Default,
	Equipment,
	Potion,
	Material,
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
	
protected:
	virtual void BeginPlay() override;
};

// 장비 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UEquipmentInventoryTypeStrategy : public UObject, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};

// 포션 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UPotionInventoryTypeStrategy : public UInventoryTypeStrategy, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};

// 재료 전략
UCLASS(BlueprintType)
class RPGPORTFOLIO_API UMaterialInventoryTypeStrategy : public UInventoryTypeStrategy, public IInventoryTypeStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory);
	virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const;
};