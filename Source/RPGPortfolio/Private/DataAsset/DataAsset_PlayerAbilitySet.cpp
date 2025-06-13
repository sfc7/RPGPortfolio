// LJS


#include "DataAsset/DataAsset_PlayerAbilitySet.h"

#include "Component/Player/PlayerUIComponent.h"
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

	for (const FPlayerAbilitySkillSet& AbilitySkillSet : PlayerInitialAbilitySkillSet)
	{
		if (!AbilitySkillSet.IsValid()) continue;

		
		FGameplayAbilitySpec AbilitySpec(AbilitySkillSet.AbilityToGrant);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySkillSet.InputTag);

		_ASC->GiveAbility(AbilitySpec);

		AActor* AvatarActor = _ASC->GetAvatarActor();
		if (AvatarActor)
		{
			UPlayerUIComponent* PlayerUIComp = AvatarActor->FindComponentByClass<UPlayerUIComponent>();
			if (PlayerUIComp)
			{
				PlayerUIComp->OnSkillIconSlotUpdatedDelegate.Broadcast(AbilitySkillSet.InputTag, AbilitySkillSet.AbilityIcon);
			}
		}
	}
}
