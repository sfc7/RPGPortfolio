// LJS


#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void URPGAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& _InputTag)
{
	if (!_InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(_InputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void URPGAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& _InputTag)
{
}
