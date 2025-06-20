// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGLevelData.generated.h"

USTRUCT(BlueprintType)
struct FLevelGameplayTag
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGLevelData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGLevelData(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag GameplayTag) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FLevelGameplayTag> GameLevelArray;
};
