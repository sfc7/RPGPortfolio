// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_StopBT.generated.h"

/**
 * UBTDeco_StopBT
 * 
 * 특정 조건에서 Behavior Tree 실행 중지
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
