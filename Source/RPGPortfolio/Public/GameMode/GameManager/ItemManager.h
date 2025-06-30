// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

struct FInventorySlot;
class UDataAsset_RPGItemData;
class UPlayerInventoryComponent;
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

	UFUNCTION(BlueprintPure)
	FInventorySlot MakeItemToAdd(TSoftObjectPtr<UDataAsset_RPGItemData> ItemToAdd, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(FInventorySlot InventorySlottoSet, int32 Index);

	UFUNCTION(BlueprintPure)
	bool IsStackableAndIsEqualAndHaveSpace(FInventorySlot& TargetSlot, FInventorySlot& SlotToStack);

	UFUNCTION(BlueprintCallable)
	void OnInventorySlotDrop(UPlayerInventoryComponent* FromContainerInventoryComponent, UPlayerInventoryComponent* ToInventoryComponent, int32 FromIndex, int32 ToIndex) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetStaciSize(FInventorySlot TargetSlot);

	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
};
