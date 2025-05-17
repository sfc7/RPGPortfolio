// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "DataAsset_MonsterAbilitySet.generated.h"

class UMonsterGameplayAbility;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_MonsterAbilitySet : public UDataAsset_AbilitySetBase
{
	GENERATED_BODY()

public:
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<TSubclassOf<UMonsterGameplayAbility>> MonsterCombatAbilities;
};
