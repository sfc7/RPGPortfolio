#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGStructTypes.h"
#include "InventorySituationStrategy.generated.h"

class UInventoryComponent;

//@ 인벤토리 상황 인터페이스 UObject 래퍼 클래스 - 언리얼 엔진 인터페이스 시스템용
UINTERFACE(Blueprintable)
class RPGPORTFOLIO_API UInventorySituationStrategy : public UInterface
{
	GENERATED_BODY()
};

//@ 인벤토리가 처한 상황 전략패턴, ex) 장비칸이 열린 상황의 인벤토리, 상점창이 열린 상황의 인벤토리
class RPGPORTFOLIO_API IInventorySituationStrategy
{
	GENERATED_BODY()
public:
	virtual void HandleItemDoubleClick(class UInventoryComponent* Inventory, FInventorySlot& SlotData) = 0;
	virtual void HandleItemRightClick(class UInventoryComponent* Inventory, FInventorySlot& SlotData) = 0;
};


