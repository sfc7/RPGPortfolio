// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_StopBT.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTDeco_StopBT : public UBTDecorator
{
	GENERATED_BODY()
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector DistToTargetKey;
	mutable bool bTargetActorDead;
	mutable bool bOwnerActorDead;
	
};
