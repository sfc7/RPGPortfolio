#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGStructTypes.h"
#include "InventoryTypeStrategy.generated.h"

class UInventoryComponent;

UINTERFACE(Blueprintable)
class RPGPORTFOLIO_API UInventoryStrategy : public UInterface
{
    GENERATED_BODY()
};

class RPGPORTFOLIO_API IInventoryStrategy
{
    GENERATED_BODY()
public:
    virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory) = 0;
    virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const = 0;
};

