#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGStructTypes.h"
#include "InventoryTypeStrategy.generated.h"

class UInventoryComponent;

//@ 인벤토리타입 인터페이스 UObject 래퍼 클래스 - 언리얼 엔진 인터페이스 시스템용
UINTERFACE(Blueprintable)
class RPGPORTFOLIO_API UInventoryTypeStrategy : public UInterface
{
    GENERATED_BODY()
};

//@ 인벤토리타입 전략패턴, ex) 장비칸, 물약칸 등
class RPGPORTFOLIO_API IInventoryTypeStrategy
{
    GENERATED_BODY()
public:
    virtual TArray<FInventorySlot>& GetSlots(UInventoryComponent* OwnerInventory) = 0;
    virtual const TArray<FInventorySlot>& GetSlots(const UInventoryComponent* OwnerInventory) const = 0;
};

