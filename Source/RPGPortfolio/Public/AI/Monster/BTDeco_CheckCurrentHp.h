// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RPGStructTypes.h"
#include "BTDeco_CheckCurrentHp.generated.h"


/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTDeco_CheckCurrentHp : public UBTDecorator
{
	GENERATED_BODY()
 protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ERPGDecoratorComparisonOperator ComparisonOperator = ERPGDecoratorComparisonOperator::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Check", meta = (SliderRange = "0, 1", ValueRange = "0, 1"), meta = (AllowPrivateAccess = "true"))
	float HpThreshHold = 0.5f;
};
