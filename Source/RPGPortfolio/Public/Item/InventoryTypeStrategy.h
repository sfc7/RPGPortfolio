// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "UObject/NoExportTypes.h"
#include "InventoryTypeStrategy.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UInventoryTypeStrategy : public UObject
{
	GENERATED_BODY()
	public:
    virtual ~UInventoryTypeStrategy() = default;
    
    virtual bool CanAddItem(const FInventorySlot& ItemToAdd, UPlayerInventoryComponent* InventoryComponent) { return true; }
    virtual bool CanEquipItem(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) { return false; }
    virtual bool CanUnEquipItem(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) { return false; }
    virtual void OnItemSet(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) {}
    virtual bool ShouldShowInUI(const FInventorySlot& ItemSlot) { return true; }
    virtual EInventoryType GetInventoryType() const PURE_VIRTUAL(UInventoryStrategy::GetInventoryType, return EInventoryType::None;);
};

UCLASS(BlueprintType)
class RPGPORTFOLIO_API UPlayerInventoryStrategy : public UInventoryTypeStrategy
{
    GENERATED_BODY()
public:
    virtual bool CanEquipItem(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) override;
    virtual EInventoryType GetInventoryType() const override { return EInventoryType::PlayerInventory; }
};

UCLASS(BlueprintType)
class RPGPORTFOLIO_API UEquipmentInventoryStrategy : public UInventoryTypeStrategy
{
    GENERATED_BODY()
public:
    virtual bool CanUnEquipItem(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) override;
    virtual void OnItemSet(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) override;
    virtual EInventoryType GetInventoryType() const override { return EInventoryType::Equipment; }
};

UCLASS(BlueprintType) 
class RPGPORTFOLIO_API UPotionInventoryStrategy : public UInventoryTypeStrategy
{
    GENERATED_BODY()
public:
    virtual void OnItemSet(const FInventorySlot& ItemSlot, UPlayerInventoryComponent* InventoryComponent) override;
    virtual bool ShouldShowInUI(const FInventorySlot& ItemSlot) override;
    virtual EInventoryType GetInventoryType() const override { return EInventoryType::Potion; }
};

UCLASS(BlueprintType)
class RPGPORTFOLIO_API UStorageInventoryStrategy : public UInventoryTypeStrategy  
{
    GENERATED_BODY()
public:
    virtual EInventoryType GetInventoryType() const override { return EInventoryType::Storage; }
};
