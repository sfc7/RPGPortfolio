// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "DataAsset_RPGItemData_Equipment.generated.h"

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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGItemData_Equipment : public UDataAsset_RPGItemData
{
	GENERATED_BODY()
public:
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
