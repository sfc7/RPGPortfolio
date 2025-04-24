// LJS


#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/MainAbilitySystemComponent.h"

void UDataAsset_AbilitySetBase::GiveToAbilitySystemComponent(UMainAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	GrantAbilities(ActivateOnGivenAbilities,InASCToGive,ApplyLevel);
	GrantAbilities(ReactiveAbilities,InASCToGive,ApplyLevel); 
}

void UDataAsset_AbilitySetBase::GrantAbilities(const TArray<TSubclassOf<URPGGameplayAbility>>& InAbilitiesToGive, UMainAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}
 
	for (const TSubclassOf<URPGGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability) continue;
 
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
 
		InASCToGive->GiveAbility(AbilitySpec);
	}	
}
