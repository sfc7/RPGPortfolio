// LJS


#include "DataAsset/DataAsset_PlayerAbilitySet.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"

void UDataAsset_PlayerAbilitySet::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	Super::GiveAbilitySystemComponent(_ASC, ApplyLevel);

	for (const FPlayerAbilitySet& AbilitySet : PlayerInitialAbilitySet)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		_ASC->GiveAbility(AbilitySpec);
	}
}
