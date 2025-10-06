
#include "GameAbilitySystem/GamePlayEffect/GEEC_DamageTaken.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "GameplayEffectTypes.h"

// 싱글톤 인스턴스로 만들어서 속성 캡처할 구조체
struct FRPGDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackRate)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)

	//@ 생성자 - Attribute 캡처 
	FRPGDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, AttackRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Damage, Target, false);
	}
};

static const FRPGDamageCapture& GetRPGDamageCapture()
{
	// FRPGDamageCapture 구조체를 싱글톤 인스턴스로 반환해야함
	static FRPGDamageCapture RPGDamageCapture;
	return RPGDamageCapture;
}

UGEEC_DamageTaken::UGEEC_DamageTaken()
{
	// 캡처할 속성들 추가
	RelevantAttributesToCapture.Add(GetRPGDamageCapture().AttackRateDef);
	RelevantAttributesToCapture.Add(GetRPGDamageCapture().DefenseDef);
	RelevantAttributesToCapture.Add(GetRPGDamageCapture().DamageDef);
}

void UGEEC_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// EvaluateParameter 설정
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 공격자의 공격률, 데미지, 콤보 캡처 
	float SourceAttackRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRPGDamageCapture().AttackRateDef, EvaluateParams,SourceAttackRate);
	float WeaponAttackRate = 1.0f; 	
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// SetByCaller GameplayTag에 캡처한 변수들 처리
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_Value_SetByCaller_WeaponAttackRate))
		{
			WeaponAttackRate = TagMagnitude.Value;
		}
		
		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_Value_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_DamageType_SetByCaller_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_DamageType_SetByCaller_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_DamageType_SetByCaller_Parrying))
		{
			BaseDamage *= 2;
		}

		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_DamageType_SetByCaller_Skill))
		{
			float SkillDamageMultiplier = TagMagnitude.Value; 
			BaseDamage *= SkillDamageMultiplier;
		}
	}

	// 타겟의 방어력 캡처
	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRPGDamageCapture().DefenseDef, EvaluateParams,TargetDefense);

	// LightAttack 콤보 처리
	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.1 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	}
	
	// HeavyAttack 콤보 처리
	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.3f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
	}

	// 최종 데미지 계산
	const float FinalDamage = BaseDamage * SourceAttackRate * WeaponAttackRate / TargetDefense;

	// 최종 데미지가 양수인 경우에만 적용
	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetRPGDamageCapture().DamageProperty,
				EGameplayModOp::Override,
				FinalDamage
			)
		);
	}
	
}
