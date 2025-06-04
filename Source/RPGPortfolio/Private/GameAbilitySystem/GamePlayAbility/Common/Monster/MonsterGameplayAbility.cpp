// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "Character/MonsterCharacter.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

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

FGameplayEffectSpecHandle UMonsterGameplayAbility::MakeMonsterBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, const FScalableFloat& _DamageScale)
{
	check(_EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle SpecHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
	_EffectClass,
	GetAbilityLevel(),
	ContextHandle);

	SpecHandle.Data->SetSetByCallerMagnitude(
		RPGGameplayTag::Data_Value_SetByCaller_BaseDamage,
		_DamageScale.GetValueAtLevel(GetAbilityLevel())
		);
	

	return SpecHandle;
}
