// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RotateToTarget.generated.h"

/**
 * 
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
	
	virtual FString GetStaticDescription() const override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	
};
