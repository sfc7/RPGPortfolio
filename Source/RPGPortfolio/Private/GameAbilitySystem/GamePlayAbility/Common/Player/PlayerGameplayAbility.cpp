// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Controller/PlayerCharacterController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetSystemLibrary.h"

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

UPlayerUIComponent* UPlayerGameplayAbility::GetPlayerUIComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent();
}

// SpecHandle에 BaseDamage는 기본으로 하여 여러 gameplaytag를 같이 보내서 데미지 최종 계산
FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerComboDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag, int32 _UsedComboCount)
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

	if (_CurrentAttackTypeTag.IsValid() && _UsedComboCount > 0)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(_CurrentAttackTypeTag, _UsedComboCount);
	}
	
	return SpecHandle;
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag)
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
		SpecHandle.Data->SetSetByCallerMagnitude(_CurrentAttackTypeTag, 0);
	}
	

	return SpecHandle;
}

bool UPlayerGameplayAbility::GetAbilityRemaningCooldownByTag(FGameplayTag CooldownTag, float& TotalCooldownTime, float& RemaningCooldownTime)
{
	if (CooldownTag.IsValid())
	{
		FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		
		TArray<TPair<float, float>> TimeRemaningAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

		if (!TimeRemaningAndDuration.IsEmpty())
		{
			RemaningCooldownTime = TimeRemaningAndDuration[0].Key;
			TotalCooldownTime = TimeRemaningAndDuration[0].Value;
		}

		return RemaningCooldownTime > 0.f;
	}

	return false;
}
