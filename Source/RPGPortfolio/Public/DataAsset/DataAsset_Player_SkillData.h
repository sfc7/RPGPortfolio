// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGStructTypes.h"
#include "DataAsset_Player_SkillData.generated.h"

/**
 * UDataAsset_Player_SkillData
 * 
 * 플레이어 캐릭터 Skill용 DataAsset (GameplayTag 및 Icon, 스킬 설명 등 정적데이터만 다룸)
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_Player_SkillData : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	TArray<FPlayerAbilitySkillSet> GetPlayerAbilitySkillSet() const { return PlayerAbilitySkillSet; }

protected:
	// FPlayerAbilitySkillSet은 GameplayTag 및 Icon, 스킬 설명 등 정적데이터만 다룸
	UPROPERTY(EditDefaultsOnly, Category = "All Skills Database")
	TArray<FPlayerAbilitySkillSet> PlayerAbilitySkillSet;
};
