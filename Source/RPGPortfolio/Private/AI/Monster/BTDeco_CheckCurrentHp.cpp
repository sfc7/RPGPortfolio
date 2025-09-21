// LJS


#include "AI/Monster/BTDeco_CheckCurrentHp.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/RPGAttributeSet.h"


bool UBTDeco_CheckCurrentHp::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return false;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!IsValid(ControlledPawn)) return false;

	URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn));
	if (!IsValid(ASC)) return false;
	
	bool bFoundCurrentHp = false;
	const float CurrentHp = ASC->GetGameplayAttributeValue(URPGAttributeSet::GetCurrentHpAttribute(), bFoundCurrentHp);

	bool bFoundMaxHp = false;
	const float MaxHp = ASC->GetGameplayAttributeValue(URPGAttributeSet::GetMaxHpAttribute(), bFoundMaxHp);
	
	if (!(bFoundCurrentHp && bFoundMaxHp) || MaxHp <= 0.0f)
	{
		return false;
	}
	
	const float HpRatio = CurrentHp / MaxHp;

	// AI의 현재 체력 비율을 ERPGDecoratorComparisonOperator에 맞게 T/F 반환
	switch (ComparisonOperator)
	{
	case ERPGDecoratorComparisonOperator::LessThan:
		return HpRatio < HpThreshHold;

	case ERPGDecoratorComparisonOperator::LessThanOrEqualTo:
		return HpRatio <= HpThreshHold;

	case ERPGDecoratorComparisonOperator::EqualTo:
		return FMath::IsNearlyEqual(HpRatio, HpThreshHold);

	case ERPGDecoratorComparisonOperator::GreaterThanOrEqualTo:
		return HpRatio >= HpThreshHold;

	case ERPGDecoratorComparisonOperator::GreaterThan:
		return HpRatio > HpThreshHold;

	default:
		return false;
	}
}
