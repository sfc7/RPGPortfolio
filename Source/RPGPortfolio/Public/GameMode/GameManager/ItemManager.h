// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

struct FInventorySlot;
class UDataAsset_RPGItemData;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UItemManager();

	UFUNCTION(BlueprintPure)
	bool IsInventorySlotEmpty(FInventorySlot InventorySlot);

	UFUNCTION(BlueprintPure)
	bool IsInventorySlotNotEmpty(FInventorySlot InventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FInventorySlot MakeItemToAdd(TSoftObjectPtr<UDataAsset_RPGItemData> ItemToAdd, int32 Quantity);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
};
