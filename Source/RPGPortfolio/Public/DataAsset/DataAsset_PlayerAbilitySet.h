// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "RPGStructTypes.h"
#include "DataAsset_PlayerAbilitySet.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_PlayerAbilitySet : public UDataAsset_AbilitySetBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);

private:
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<FPlayerAbilitySet> PlayerInitialAbilitySet;

	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<FPlayerAbilitySkillSet> PlayerInitialAbilitySkillSet;
};
