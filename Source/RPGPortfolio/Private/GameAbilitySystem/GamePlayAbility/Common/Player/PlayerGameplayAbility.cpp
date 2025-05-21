// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Controller/PlayerCharacterController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

APlayerCharacterBase* UPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!PlayerCharacter.IsValid())
	{
		PlayerCharacter = CastChecked<APlayerCharacterBase>(CurrentActorInfo->AvatarActor);
	}
	
	return PlayerCharacter.IsValid() ? PlayerCharacter.Get() : nullptr;
}

APlayerCharacterController* UPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!PlayerCharacterController.IsValid())
	{
		PlayerCharacterController = CastChecked<APlayerCharacterController>(CurrentActorInfo->PlayerController);
	}
	
	return PlayerCharacterController.IsValid() ? PlayerCharacterController.Get() : nullptr;
}

UPlayerCombatComponent* UPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag, int32 _UsedComboCount)
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
		_WeaponBaseDamage
	);

	if (_CurrentAttackTypeTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(_CurrentAttackTypeTag, _UsedComboCount);
	}
	
	return SpecHandle;
}
