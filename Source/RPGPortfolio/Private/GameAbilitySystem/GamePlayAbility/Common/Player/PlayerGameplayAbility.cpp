// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Controller/PlayerCharacterController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameAbilitySystem/RPGAttributeSet.h"

APlayerCharacterBase* UPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	// 캐시된 캐릭터가 유효한 경우 원래 캐릭터 반환
	if (PlayerCharacter.IsValid()) return PlayerCharacter.Get();

	if (!CurrentActorInfo) return nullptr;
        
	if (!CurrentActorInfo->AvatarActor.IsValid()) return nullptr;

	// CurrentActorInfo의 아바타 액터로 PlayerCharacter 참조
	PlayerCharacter = Cast<APlayerCharacterBase>(CurrentActorInfo->AvatarActor);
    
	return PlayerCharacter.IsValid() ? PlayerCharacter.Get() : nullptr;
}

APlayerCharacterController* UPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	// 캐시된 컨트롤러가 유효한 경우 원래 컨트롤러 반환
	if (PlayerCharacterController.IsValid()) return PlayerCharacterController.Get();

	// CurrentActorInfo의 컨트롤러로 PlayerController 참조
	PlayerCharacterController = CastChecked<APlayerCharacterController>(CurrentActorInfo->PlayerController);
	
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


FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerComboDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponAttackRate,float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag, int32 _UsedComboCount)
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
		_WeaponBaseDamage
	);

	// 무기의 AttackRate 설정
	SpecHandle.Data->SetSetByCallerMagnitude(
	RPGGameplayTag::Data_Value_SetByCaller_WeaponAttackRate,
	_WeaponAttackRate
	);
	
	// 콤보 태그 및 카운트 설정
	if (_CurrentAttackTypeTag.IsValid() && _UsedComboCount > 0)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(_CurrentAttackTypeTag, _UsedComboCount);
	}
	
	return SpecHandle;
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerBaseDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponAttackRate, float _WeaponBaseDamage, FGameplayTag _CurrentAttackTypeTag)
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
		_WeaponBaseDamage
		);

	// 무기의 AttackRate 설정
	SpecHandle.Data->SetSetByCallerMagnitude(
	RPGGameplayTag::Data_Value_SetByCaller_WeaponAttackRate,
	_WeaponAttackRate
	);

	// 공격 타입 태그 설정
	if (_CurrentAttackTypeTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(_CurrentAttackTypeTag, 0);
	}
	

	return SpecHandle;
}

FGameplayEffectSpecHandle UPlayerGameplayAbility::MakePlayerSkillDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> _EffectClass, float _WeaponBaseDamage, const FScalableFloat& _SkillDamageMultiplier)
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
	GetAbilityLevel(), // Todo Skill Level Apply 
	ContextHandle);

	// 기본 데미지 설정
	SpecHandle.Data->SetSetByCallerMagnitude(
	RPGGameplayTag::Data_Value_SetByCaller_BaseDamage,
	_WeaponBaseDamage
	);

	// 스킬 데미지 배율 설정
	SpecHandle.Data->SetSetByCallerMagnitude(
		RPGGameplayTag::Data_DamageType_SetByCaller_Skill,
		_SkillDamageMultiplier.GetValueAtLevel(GetAbilityLevel())
	);

	return SpecHandle;
}

bool UPlayerGameplayAbility::GetAbilityRemaningCooldownByTag(FGameplayTag CooldownTag, float& TotalCooldownTime, float& RemaningCooldownTime)
{
	if (!CooldownTag.IsValid()) return false;
	
	// 쿨다운 GameplayEffectQuery 생성
	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	
	// GameplayEffectQuery에서 쿨다운 남은시간, 지속시간 가져오기
	TArray<TPair<float, float>> TimeRemaningAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	// 쿨다운 GameplayEffect가 있는 경우
	if (!TimeRemaningAndDuration.IsEmpty())
	{
		RemaningCooldownTime = TimeRemaningAndDuration[0].Key;
		TotalCooldownTime = TimeRemaningAndDuration[0].Value;
	}

	return RemaningCooldownTime > 0.f;
}

FString UPlayerGameplayAbility::GetDescriptionForUI()
{
	const int32 CurrentLevel = GetAbilityLevel();

	//GameplayEffect로 CostMana와 Cooldown 가져와서 String 생성
	const float CostMana = FMath::Abs(GetCostMana(CurrentLevel));
	const float Cooldown = GetCooldown(CurrentLevel);
	
	return *FString::Printf(TEXT("소모마나 %.1f 쿨다운 %.1f"), CostMana, Cooldown);
}

float UPlayerGameplayAbility::GetCostMana(float InLevel)
{
	float cost = 0.f;
	
	// 코스트 GameplayEffect 가져오기
	const UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (!IsValid(CostEffect)) return cost;

	// UGameplayEffect에서 마나 속성 찾기
	for (const FGameplayModifierInfo& ModifierInfo : CostEffect->Modifiers)
	{
		if (ModifierInfo.Attribute == URPGAttributeSet::GetCurrentMpAttribute())
		{
			ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, cost);
			break;
		}
	}
	
	return cost;
}

float UPlayerGameplayAbility::GetCooldown(float InLevel)
{
	float cooldown = 0.f;
	
	// 쿨다운 GameplayEffect 가져오기
	const UGameplayEffect* CooldownCost = GetCooldownGameplayEffect();
	if (!IsValid(CooldownCost)) return cooldown;

	// 쿨다운 시간 계산
	CooldownCost->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, cooldown);
	
	return cooldown;
}
