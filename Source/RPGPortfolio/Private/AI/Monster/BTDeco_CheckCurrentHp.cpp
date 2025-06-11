// LJS


#include "AI/Monster/BTDeco_CheckCurrentHp.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/RPGAttributeSet.h"


bool UBTDeco_CheckCurrentHp::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}

	URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn));
	if (ASC)
	{
		bool bFoundCurrentHp;
		float CurrentHp = ASC->GetGameplayAttributeValue(URPGAttributeSet::GetCurrentHpAttribute(), bFoundCurrentHp);

		bool bFoundCMaxHp;
		float MaXHp = ASC->GetGameplayAttributeValue(URPGAttributeSet::GetCurrentHpAttribute(), bFoundCMaxHp);

		if (bFoundCurrentHp && bFoundCMaxHp)
		{
			float Hpratio = CurrentHp / MaXHp;

			switch (ComparisonOperator)
			{
			case ERPGDecoratorComparisonOperator::LessThan:
				return Hpratio < HpThreshHold;
			case ERPGDecoratorComparisonOperator::LessThanOrEqualTo:
				return Hpratio <= HpThreshHold;
			case ERPGDecoratorComparisonOperator::EqualTo:
				return FMath::IsNearlyEqual(Hpratio, HpThreshHold);
			case ERPGDecoratorComparisonOperator::GreaterThanOrEqualTo:
				return Hpratio >= HpThreshHold;
			case ERPGDecoratorComparisonOperator::GreaterThan:
				return Hpratio > HpThreshHold;
			default:
				return false;
			}
		}
	}


	return false;
}
