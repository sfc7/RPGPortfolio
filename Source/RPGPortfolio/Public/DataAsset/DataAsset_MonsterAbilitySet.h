// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "DataAsset_MonsterAbilitySet.generated.h"

class UMonsterGameplayAbility;
/**
 * UDataAsset_MonsterAbilitySet
 * 
 * 몬스터 GameplayAbility용 DataAsset
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_MonsterAbilitySet : public UDataAsset_AbilitySetBase
{
	GENERATED_BODY()

public:
	//@ ASC에 몬스터 Ability들 부여
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
	
private:
	//@ 몬스터 GameplayAbility
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<TSubclassOf<UMonsterGameplayAbility>> MonsterCombatAbilities;
};
