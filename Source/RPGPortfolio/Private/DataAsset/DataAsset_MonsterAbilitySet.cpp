// LJS


#include "DataAsset/DataAsset_MonsterAbilitySet.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UDataAsset_MonsterAbilitySet::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	if (!IsValid(_ASC)) return;

	// 부모 클래스의 기본 어빌리티들 먼저 부여 (패시브, 반응형 등)
	Super::GiveAbilitySystemComponent(_ASC, ApplyLevel);
	
	if (MonsterCombatAbilities.IsEmpty()) return;

	// 모든 몬스터 전투 어빌리티를 ASC에 부여
	for (const TSubclassOf<UMonsterGameplayAbility>& AbilityClass : MonsterCombatAbilities)
	{
		if (!IsValid(AbilityClass)) continue;

		// 어빌리티 스펙 생성 (일단 동적 태그 없게 설계)
		FGameplayAbilitySpec AbilitySpec(AbilityClass);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		// ASC에 어빌리티 부여
		_ASC->GiveAbility(AbilitySpec);
	}
}
