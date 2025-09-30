// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "DataAsset_RPGItemData_Equipment.generated.h"

//@ 장비 타입
UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Helmet,
	Armor,
	Pants,
	Gloves,
	Boots,
	None
};	
/**
 * UDataAsset_RPGItemData_Equipment
 * 
 * 장비 아이템
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGItemData_Equipment : public UDataAsset_RPGItemData
{
	GENERATED_BODY()
public:
	//@ 장비 타입
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EEquipmentType EquipmentType;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MaxHp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MaxMp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float AttackRate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Defense;
};
