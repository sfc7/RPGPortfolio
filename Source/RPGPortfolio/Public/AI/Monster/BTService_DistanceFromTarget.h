// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DistanceFromTarget.generated.h"

/**
 * UBTService_DistanceFromTarget
 * 
 * 0.2초마다 타겟까지 거리 계산 후 Blackboard에 기록
 */
UCLASS()
class RPGPORTFOLIO_API UBTService_DistanceFromTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DistanceFromTarget();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceFromTarget;



};