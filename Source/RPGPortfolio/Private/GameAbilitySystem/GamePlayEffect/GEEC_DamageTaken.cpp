
#include "GameAbilitySystem/GamePlayEffect/GEEC_DamageTaken.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/RPGAttributeSet.h"
#include "GameplayEffectTypes.h"

struct FRPGDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackRate)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)

	FRPGDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, AttackRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Damage, Target, false);
	}
};

static const FRPGDamageCapture& GetRPGDamageCapture()
{
	static FRPGDamageCapture RPGDamageCapture;
	return RPGDamageCapture;
}

UGEEC_DamageTaken::UGEEC_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetRPGDamageCapture().AttackRateDef);
	RelevantAttributesToCapture.Add(GetRPGDamageCapture().DefenseDef);
	RelevantAttributesToCapture.Add(GetRPGDamageCapture().DamageDef);
}

void UGEEC_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// EffectSpec.GetContext().GetSourceObject();
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRPGDamageCapture().AttackRateDef, EvaluateParams,SourceAttackRate);
	
	
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_Value_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			// UE_LOG(LogTemp,Log,TEXT("BaseDamage : %f"), BaseDamage);
		}

		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_DamageType_SetByCaller_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(RPGGameplayTag::Data_DamageType_SetByCaller_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}
	}
	
	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRPGDamageCapture().DefenseDef, EvaluateParams,TargetDefense);
	
	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.1 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.3f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
	}

	const float FinalDamage = BaseDamage * SourceAttackRate / TargetDefense;

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
	// UE_LOG(LogTemp,Log,TEXT("FinalDamage : %f"), FinalDamage);
	
}
