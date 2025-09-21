// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RotateToTarget.generated.h"

/**
 * UBTService_RotateToTarget
 * 
 * 타겟을 바라보도록 Tick마다 회전 보간
 */
UCLASS()
class RPGPORTFOLIO_API UBTService_RotateToTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_RotateToTarget();
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 디버그 설명
	virtual FString GetStaticDescription() const override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	// 회전 속도
	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	
};
