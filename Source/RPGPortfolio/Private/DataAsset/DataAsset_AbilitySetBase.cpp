// LJS


#include "DataAsset/DataAsset_AbilitySetBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

void UDataAsset_AbilitySetBase::GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	if (!IsValid(_ASC)) return;

	//@ 부여 시 즉시 활성화되는 어빌리티들, 패시브 스킬 등 부여
	GrantAbilities(ActivateOnGivenAbilities, _ASC, ApplyLevel);
	//@ 반응형 어빌리티들, 특정 조건 트리거 발동형 부여
	GrantAbilities(ReactiveAbilities, _ASC, ApplyLevel);

	if (InitGameplayEffects.IsEmpty()) return;

	// 초기 게임플레이 이펙트들 적용
	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitGameplayEffects)
	{
		if (!IsValid(EffectClass)) continue;

		// 이펙트 컨텍스트 생성
		FGameplayEffectContextHandle EffectContextHandle = _ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(_ASC->GetAvatarActor());
		
		// 이펙트 스펙 생성
		const FGameplayEffectSpecHandle EffectSpecHandle = _ASC->MakeOutgoingSpec(EffectClass, ApplyLevel, EffectContextHandle);
		if (!EffectSpecHandle.IsValid()) continue;

		// 자신에게 이펙트 적용
		_ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void UDataAsset_AbilitySetBase::GrantAbilities(const TArray<TSubclassOf<URPGGameplayAbility>>& _ToBeGrantedAbilities, URPGAbilitySystemComponent* _ASC, int32 ApplyLevel)
{
	if (_ToBeGrantedAbilities.IsEmpty()) return;

	if (!IsValid(_ASC)) return;
 
	// 모든 어빌리티를 ASC에 부여
	for (const TSubclassOf<URPGGameplayAbility>& Ability : _ToBeGrantedAbilities)
	{
		if (!IsValid(Ability)) continue;
 
		// 어빌리티 스펙 생성
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = _ASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
 
		// ASC에 어빌리티 부여
		_ASC->GiveAbility(AbilitySpec);
	}	
}