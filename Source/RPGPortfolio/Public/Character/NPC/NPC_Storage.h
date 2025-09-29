// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/RPGNPCCharacterBase.h"
#include "NPC_Storage.generated.h"

class UInventoryComponent;
/**
 * ANPC_Storage
 * 
 * NPC Character의 타입 중 창고
 */
UCLASS()
class RPGPORTFOLIO_API ANPC_Storage : public ARPGNPCCharacterBase
{
	GENERATED_BODY()
public:
	ANPC_Storage();

	//@ 상호작용 당할 때 보여줄 창고의 UI
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetStorageWidget();

private:
	//@ 창고의 인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* StorageInventory;
};
