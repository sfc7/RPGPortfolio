#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGStructTypes.h"
#include "InventorySituationStrategy.generated.h"

class UInventoryComponent;

UINTERFACE(Blueprintable)
class RPGPORTFOLIO_API UInventorySituationStrategy : public UInterface
{
	GENERATED_BODY()
};

class RPGPORTFOLIO_API IInventorySituationStrategy
{
	GENERATED_BODY()
public:
	virtual void HandleItemDoubleClick(class UInventoryComponent* Inventory, FInventorySlot& SlotData) = 0;
	virtual void HandleItemRightClick(class UInventoryComponent* Inventory, FInventorySlot& SlotData) = 0;
};


