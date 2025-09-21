// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RPGStructTypes.h"
#include "BTDeco_CheckCurrentHp.generated.h"


/**
 * UBTDeco_CheckCurrentHp
 * 
 * 현재 AI의 HP 비율을 체크하고 설정된 조건(ComparisonOperator, HpThreshHold)에 따라 true/false 반환
 */
UCLASS()
class RPGPORTFOLIO_API UBTDeco_CheckCurrentHp : public UBTDecorator
{
	GENERATED_BODY()
 protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:

	// HpThreshHold를 비교하는 기준 Enum
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ERPGDecoratorComparisonOperator ComparisonOperator = ERPGDecoratorComparisonOperator::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Check", meta = (SliderRange = "0, 1", ValueRange = "0, 1"), meta = (AllowPrivateAccess = "true"))
	float HpThreshHold = 0.5f;
};
