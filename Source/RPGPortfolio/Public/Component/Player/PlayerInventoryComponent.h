// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGStructTypes.h" 
#include "PlayerInventoryComponent.generated.h"


class UPlayerInventoryComponent;
class UDataAsset_RPGItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChangedDelegate, FInventorySlot, Slot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void SetupSlots(int32 SlotAmountstoSetup);

	UFUNCTION(BlueprintCallable)
	bool AddItem(FInventorySlot ItemToAdd);

	UFUNCTION(BlueprintCallable)
	void SetItem(FInventorySlot TargetSlot, FInventorySlot ItemToSet);

	UFUNCTION(BlueprintCallable)
	bool FindEmptySlot(FInventorySlot& OutEmptySlot);
	
	FOnInventorySlotChangedDelegate OnInventorySlotChangedDelegate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	int32 SlotAmounts;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Size")
	TArray<FInventorySlot> ItemSlots;

	// UFUNCTION(Blueprintable)
	// void SetCurrentInventoryWidget();
	
protected:
	virtual void BeginPlay() override;

	UUserWidget* InventoryWidget;
};
