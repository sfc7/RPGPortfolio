// LJS


#include "DataAsset/DataAsset_PlayerAbilitySet.h"

#include "Component/Player/PlayerUIComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"

void UDataAsset_PlayerAbilitySet::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	if (!IsValid(_ASC)) return;

	// 부모 클래스의 기본 어빌리티들 먼저 부여 (패시브, 반응형 등)
	Super::GiveAbilitySystemComponent(_ASC, ApplyLevel);
	
	if (PlayerInitialAbilitySet.IsEmpty()) return;

	// 모든 플레이어 어빌리티를 동적 어빌리티로 ASC에 부여
	for (const FPlayerAbilitySet& AbilitySet : PlayerInitialAbilitySet)
	{
		if (!AbilitySet.IsValid()) continue;

		// 동적 어빌리티 스펙 생성 (입력 태그와 함께)
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		// 동적 소스 태그에 입력 태그 추가 (키 바인딩 가능하게 만듬)
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		// ASC에 동적 어빌리티 부여
		_ASC->GiveAbility(AbilitySpec);
	}
}