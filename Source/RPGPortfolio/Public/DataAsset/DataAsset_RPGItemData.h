// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGItemData.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGItemData(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsStackable();

	UFUNCTION(BlueprintCallable)
	FName GetItemID() const {return ItemID; }

	UFUNCTION(BlueprintCallable)
	UDataAsset_RPGItemData* GetItemAsset() { return this; }
	
	UFUNCTION(BlueprintCallable)
	void IsItemStackable(bool& IsStackable, int32& StackSize);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName ItemID;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ItemName;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText ItemDescription;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 StackSize;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Weight;
};
