// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "DataAsset_AbilitySetBase.generated.h"

class URPGGameplayAbility;
class URPGAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_AbilitySetBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
 
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray< TSubclassOf <URPGGameplayAbility>> ActivateOnGivenAbilities;
 
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray< TSubclassOf <URPGGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<TSubclassOf<UGameplayEffect>> InitGameplayEffects;
 
	void GrantAbilities(const TArray< TSubclassOf <URPGGameplayAbility> >& _ToBeGrantedAbilities,URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
};

