// LJS


#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UDataAsset_AbilitySetBase::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	GrantAbilities(ActivateOnGivenAbilities,_ASC,ApplyLevel);
	GrantAbilities(ReactiveAbilities,_ASC,ApplyLevel); 
}

void UDataAsset_AbilitySetBase::GrantAbilities(const TArray<TSubclassOf<URPGGameplayAbility>>& _ToBeGrantedAbilities, URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	if (_ToBeGrantedAbilities.IsEmpty())
	{
		return;
	}
 
	for (const TSubclassOf<URPGGameplayAbility>& Ability : _ToBeGrantedAbilities)
	{
		if(!Ability) continue;
 
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
 
		_ASC->GiveAbility(AbilitySpec);
	}	
}
