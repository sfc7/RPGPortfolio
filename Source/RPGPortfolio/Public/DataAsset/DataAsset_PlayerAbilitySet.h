// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameplayTagContainer.h"
#include "DataAsset_PlayerAbilitySet.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAbilitySet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URPGGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_PlayerAbilitySet : public UDataAsset_AbilitySetBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
	
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<FPlayerAbilitySet> PlayerInitialAbilitySet;
	
	
	
};
