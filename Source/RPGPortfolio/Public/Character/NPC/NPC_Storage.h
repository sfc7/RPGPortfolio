// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/RPGNPCCharacterBase.h"
#include "NPC_Storage.generated.h"

class UInventoryComponent;
/**
 ** 
 */
UCLASS()
class RPGPORTFOLIO_API ANPC_Storage : public ARPGNPCCharacterBase
{
	GENERATED_BODY()
public:
	ANPC_Storage();
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetStorageWidget();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* StorageInventory;
};
