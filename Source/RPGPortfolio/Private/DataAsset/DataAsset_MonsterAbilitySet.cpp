// LJS


#include "DataAsset/DataAsset_MonsterAbilitySet.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UDataAsset_MonsterAbilitySet::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	Super::GiveAbilitySystemComponent(_ASC, ApplyLevel);

	if (!MonsterCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UMonsterGameplayAbility> & AbilityClass : MonsterCombatAbilities)
		{
			if (!AbilityClass) continue;


			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = _ASC->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			
			_ASC->GiveAbility(AbilitySpec);
		}
	}
}
