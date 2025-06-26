// LJS


#include "DataAsset/DataAsset_RPGItemData.h"

UDataAsset_RPGItemData::UDataAsset_RPGItemData(const FObjectInitializer& ObjectInitializer)
{
}

bool UDataAsset_RPGItemData::IsStackable()
{
	return StackSize > 0.f;
}

void UDataAsset_RPGItemData::IsItemStackable(bool& IsStackableCheck, int32& StackSizeCheck) 
{
	IsStackableCheck = IsStackable();
	StackSize = StackSizeCheck;
}
