// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGStructTypes.h"
#include "DataAsset_Player_SkillData.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_Player_SkillData : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	TArray<FPlayerAbilitySkillSet> GetPlayerAbilitySkillSet() const { return PlayerAbilitySkillSet; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "All Skills Database")
	TArray<FPlayerAbilitySkillSet> PlayerAbilitySkillSet;
};
