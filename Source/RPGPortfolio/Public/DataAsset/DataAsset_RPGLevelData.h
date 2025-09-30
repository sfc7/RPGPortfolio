// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGLevelData.generated.h"

//@ 레벨 GameplayTag 구조체 - 레벨 태그와 월드 레벨 매핑
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
 * UDataAsset_RPGLevelData
 * 
 * 게임 레벨 관리 데이터 애셋, 던전, 마을, 필드 등 레벨 관리용
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGLevelData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGLevelData(const FObjectInitializer& ObjectInitializer);

	//@ 게임플레이 태그로 해당하는 게임 레벨 검색
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag GameplayTag) const;
	
protected:
	//@ 게임 레벨 배열 (태그와 월드 레벨 매핑)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FLevelGameplayTag> GameLevelArray;
};
