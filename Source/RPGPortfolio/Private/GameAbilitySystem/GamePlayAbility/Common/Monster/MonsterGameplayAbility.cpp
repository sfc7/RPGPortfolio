// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "Character/MonsterCharacter.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

AMonsterCharacter* UMonsterGameplayAbility::GetMonsterCharacterFromActorInfo()
{
	// 캐시된 몬스터 캐릭터가 유효한 경우 반환
	if (MonsterCharacter.IsValid()) return MonsterCharacter.Get();

	if (!CurrentActorInfo) return nullptr;
        
	if (!CurrentActorInfo->AvatarActor.IsValid()) return nullptr;
	
	// CurrentActorInfo의 아바타 액터로 몬스터 참조
	MonsterCharacter = CastChecked<AMonsterCharacter>(CurrentActorInfo->AvatarActor);
	
	return MonsterCharacter.IsValid() ? MonsterCharacter.Get() : nullptr; 
}

UMonsterCombatComponent* UMonsterGameplayAbility::GetMonsterCombatComponentFromActorInfo()
{
	return GetMonsterCharacterFromActorInfo()->GetMonsterCombatComponent();
}

FGameplayEffectSpecHandle UMonsterGameplayAbility::MakeMonsterBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, const FScalableFloat& _DamageScale)
{
	check(_EffectClass);

	// 이펙트 컨텍스트 생성
	FGameplayEffectContextHandle ContextHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 이펙트 스펙에 Context 탑재
	FGameplayEffectSpecHandle SpecHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
	_EffectClass,
	GetAbilityLevel(),
	ContextHandle);

	// 기본 데미지 설정
	SpecHandle.Data->SetSetByCallerMagnitude(
		RPGGameplayTag::Data_Value_SetByCaller_BaseDamage,
		_DamageScale.GetValueAtLevel(GetAbilityLevel())
		);
	

	return SpecHandle;
}
