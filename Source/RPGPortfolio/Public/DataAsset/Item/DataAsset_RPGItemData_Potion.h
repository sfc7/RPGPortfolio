// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "DataAsset_RPGItemData_Potion.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGItemData_Potion : public UDataAsset_RPGItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 HealAmount;
};
