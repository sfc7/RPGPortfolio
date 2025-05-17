// LJS


#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UDataAsset_AbilitySetBase::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	GrantAbilities(ActivateOnGivenAbilities,_ASC,ApplyLevel);
	GrantAbilities(ReactiveAbilities,_ASC,ApplyLevel);

	if (!InitGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : InitGameplayEffects)
		{
			if (!EffectClass) continue;

			FGameplayEffectContextHandle EffectContextHandle = _ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(_ASC->GetAvatarActor());
			FGameplayEffectSpecHandle EffectSpecHandle = _ASC->MakeOutgoingSpec(EffectClass, ApplyLevel, EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				_ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			}
		}
	}
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
