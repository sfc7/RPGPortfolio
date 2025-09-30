// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/InventoryComponent.h"
#include "PlayerEquipmentComponent.generated.h"

/**
 * UPlayerEquipmentComponent
 * 
 * 장비 인벤토리에 사용할 컴포넌트
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
	//@ 장비 타입에 따른 슬롯 인덱스와 포인터 반환
	bool GetEquipmentSlotInfo(EEquipmentType EquipmentType, int32& OutSlotIndex, FInventorySlot*& OutTargetSlot);

	//@ 기존 장비 제거 및 인벤토리에 반환
	void RemoveExistingEquipment(FInventorySlot* TargetSlot);

	//@ 장비 슬롯에 새 아이템 설정
	void SetEquipmentSlot(const FInventorySlot& ItemToSet, int32 SlotIndex, FInventorySlot* TargetSlot);

	//@ 장비 능력치 적용
	void ApplyEquipmentStats(const UDataAsset_RPGItemData_Equipment* EquipmentData);

	//@ 장비 능력치 제거
	void RemoveEquipmentStats(const UDataAsset_RPGItemData_Equipment* EquipmentData);

	//@ 해제된 장비를 인벤토리에 반환
	void ReturnEquipmentToInventory(const FInventorySlot& EquipmentSlot);

	//@ 장비 슬롯 초기화
	void ClearEquipmentSlot(int32 SlotIndex, FInventorySlot* TargetSlot);
private:
	//@ 장비 부위별 Slot
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

	//@ 플레이어 인벤토리 참조
	UInventoryComponent* PlayerInventoryComponentRef;
};
