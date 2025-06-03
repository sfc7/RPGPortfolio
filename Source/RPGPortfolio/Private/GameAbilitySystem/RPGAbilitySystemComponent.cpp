// LJS


#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

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
	if (!_InputTag.IsValid() || !_InputTag.MatchesTag(RPGGameplayTag::InputTag_Hold))
	{
		return;
	}
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(_InputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

bool URPGAbilitySystemComponent::ActivateGamePlayAbilityByTag(FGameplayTag _ActivateAbilityTag)
{
	check(_ActivateAbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> CanActiveAbilitySpecArray;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(_ActivateAbilityTag.GetSingleTagContainer(),CanActiveAbilitySpecArray);

	if (!CanActiveAbilitySpecArray.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, CanActiveAbilitySpecArray.Num()-1);
		FGameplayAbilitySpec* CanActiveAbilitySpec = CanActiveAbilitySpecArray[RandomIndex];
		if (CanActiveAbilitySpec && !CanActiveAbilitySpec->IsActive())
		{
			return TryActivateAbility(CanActiveAbilitySpec->Handle);
		}
	}

	return false;
}
