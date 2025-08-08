// LJS


#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "DataAsset/SaveGame/RPGSaveGame.h"

void URPGAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& _InputTag)
{
	if (!_InputTag.IsValid())
	{
		return;
	}
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(_InputTag)) continue;

		if (_InputTag.MatchesTag(RPGGameplayTag::InputTag_Toggle))
		{
			if (AbilitySpec.IsActive())
			{
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
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

FGameplayAbilitySpec* URPGAbilitySystemComponent::FindActiveAbilityByTag(FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		return nullptr;
	}

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		for (const FGameplayTag& AssetTag : Spec.Ability->GetAssetTags())
		{
			if (AssetTag.MatchesTagExact(AbilityTag))
			{
				return &Spec;
			}
		}
	}
	
	return nullptr;
}

TArray<FGameplayAbilitySpec> URPGAbilitySystemComponent::GetAllSkillAbilities() const
{
	TArray<FGameplayAbilitySpec> SkillAbilities;

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		for (const FGameplayTag& Tag : Spec.Ability->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Player.Ability.Skill")))
			{
				SkillAbilities.Add(Spec);
				break;
			}
		}		
	}

	return SkillAbilities;
}

void URPGAbilitySystemComponent::SaveDynamicAbilitiesToSaveGame(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;

	TArray<FAbilitySpecSaveData> DynamicAbilities;
    
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (IsDynamicAbility(Spec))
		{
			FAbilitySpecSaveData SaveData;
			SaveData.AbilityClass = Spec.Ability->GetClass();
			SaveData.Level = Spec.Level;
            
			for (const FGameplayTag& Tag : Spec.GetDynamicSpecSourceTags())
			{
				SaveData.DynamicSpecSourceTags.Add(Tag);
			}
            
			DynamicAbilities.Add(SaveData);
		}
	}
    
	SaveGame->SetPlayerDynamicAbilities(DynamicAbilities);
}

void URPGAbilitySystemComponent::LoadDynamicAbilitiesFromSaveGame(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (IsDynamicAbility(Spec))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
    
	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToRemove)
	{
		ClearAbility(Handle);
	}
    
	TArray<FAbilitySpecSaveData> DynamicAbilities = SaveGame->GetPlayerDynamicAbilities();
	for (const FAbilitySpecSaveData& SaveData : DynamicAbilities)
	{
		if (SaveData.AbilityClass.IsValid())
		{
			UClass* AbilityClass = SaveData.AbilityClass.LoadSynchronous();
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass);
				AbilitySpec.SourceObject = GetAvatarActor();
				AbilitySpec.Level = SaveData.Level;
                
				for (const FGameplayTag& Tag : SaveData.DynamicSpecSourceTags)
				{
					AbilitySpec.GetDynamicSpecSourceTags().AddTag(Tag);
				}
                
				GiveAbility(AbilitySpec);
			}
		}
	}
}

bool URPGAbilitySystemComponent::IsDynamicAbility(const FGameplayAbilitySpec& Spec) const
{
	return Spec.GetDynamicSpecSourceTags().Num() > 0;
}
