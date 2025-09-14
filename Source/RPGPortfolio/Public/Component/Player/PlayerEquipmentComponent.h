// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/InventoryComponent.h"
#include "PlayerEquipmentComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()
public:
	UPlayerEquipmentComponent();

	UFUNCTION(BlueprintCallable)
	void ApplyEquipmentItem(FInventorySlot ItemToSet);

	UFUNCTION(BlueprintCallable)
	void ApplyUnEquipmentItem(FInventorySlot ItemToSet);
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FInventorySlot HelmetSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FInventorySlot ArmorSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FInventorySlot PantsSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FInventorySlot GlovesSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FInventorySlot BootsSlot;

	UInventoryComponent* PlayerInventoryComponentRef;
};
