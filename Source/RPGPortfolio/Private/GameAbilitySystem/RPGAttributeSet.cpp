// LJS


#include "GameAbilitySystem/RPGAttributeSet.h"
#include "GameplayEffectExtension.h"


URPGAttributeSet::URPGAttributeSet()
{
	InitCurrentHp(1.f),
	InitMaxHp(1.f),
	InitCurrentMp(1.f),
	InitMaxMp(1.f),
	InitAttackRate(1.f),
	InitDefense(1.f);
	InitDamage(1.f);
	
}

void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		float NewCurrentHp = FMath::Clamp(GetCurrentHp(), 0.f, GetMaxHp());

		SetCurrentHp(NewCurrentHp);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentMpAttribute())
	{
		float NewCurrentMp = FMath::Clamp(GetCurrentMp(), 0.f, GetMaxMp());

		SetCurrentHp(NewCurrentMp);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float PreCurrentHp = GetCurrentHp();
		float CalcDamage = GetDamage();

		float CalcCurrentHp = FMath::Clamp(PreCurrentHp - CalcDamage, 0.f, GetMaxHp());

		SetCurrentHp(CalcCurrentHp);
	}
}

