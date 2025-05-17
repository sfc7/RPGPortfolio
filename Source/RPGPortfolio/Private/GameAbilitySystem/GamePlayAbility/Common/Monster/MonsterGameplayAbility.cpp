// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "Character/MonsterCharacter.h"

AMonsterCharacter* UMonsterGameplayAbility::GetMonsterCharacterFromActorInfo()
{
	if (!MonsterCharacter.IsValid())
	{
		MonsterCharacter = CastChecked<AMonsterCharacter>(CurrentActorInfo->AvatarActor);
	}

	return MonsterCharacter.IsValid() ? MonsterCharacter.Get() : nullptr; 
}

UMonsterCombatComponent* UMonsterGameplayAbility::GetMonsterCombatComponentFromActorInfo()
{
	return GetMonsterCharacterFromActorInfo()->GetMonsterCombatComponent();
}
