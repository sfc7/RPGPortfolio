// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "DataAsset_AbilitySetBase.generated.h"

class URPGGameplayAbility;
class UMainAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_AbilitySetBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UMainAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1);
 
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf <URPGGameplayAbility>> ActivateOnGivenAbilities;
 
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf <URPGGameplayAbility>> ReactiveAbilities; // 언젠가 이름 바꿀수도 Reactive가 무슨뜻인지 안 와닿음
 
	void GrantAbilities(const TArray< TSubclassOf <URPGGameplayAbility> >& InAbilitiesToGive,UMainAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1);
};

