// LJS


#include "RPGStructTypes.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"

bool FPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
