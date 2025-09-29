// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/RPGNPCCharacterBase.h"
#include "NPC_HumanNPC.generated.h"

class AQuestNPCComponent; 
/**
 * ARPGCharacterBase
 * 
 * NPC Character의 타입 중 인간
 */
UCLASS()
class RPGPORTFOLIO_API ANPC_HumanNPC : public ARPGNPCCharacterBase
{
	GENERATED_BODY()
public:
	ANPC_HumanNPC();

	//@ 상호작용 당할 때 보여줄 UI
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetStoreWidget();

	virtual UInventoryComponent* GetNPCInventoryComponent() const { return StoreInventory; }
	
private:
	//@ NPC의 인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* StoreInventory;
};
