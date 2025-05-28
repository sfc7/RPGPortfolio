// LJS


#include "AI/Monster/BTDeco_RandomProbability.h"
#include "Kismet/KismetMathLibrary.h"

bool UBTDeco_RandomProbability::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	float RandomValue = FMath::RandRange(MinValue, MaxValue);
	bool bSuccess = UKismetMathLibrary::RandomBoolWithWeight(RandomValue);
	
	return bSuccess;
}
