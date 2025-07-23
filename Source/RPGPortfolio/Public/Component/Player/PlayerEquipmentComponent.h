// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "PlayerEquipmentComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerEquipmentComponent : public UPlayerInventoryComponent
{
	GENERATED_BODY()
public:
	UPlayerEquipmentComponent();

	UFUNCTION(BlueprintCallable)
	void ApplyEquipmentItem(FInventorySlot ItemToSet);
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
};
