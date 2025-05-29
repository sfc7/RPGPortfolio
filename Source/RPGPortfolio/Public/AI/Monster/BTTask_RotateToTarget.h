// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToTarget.generated.h"

struct FRotateToTargetTaskInstance
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTTask_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_RotateToTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

	bool HasReachedAnglePercision(APawn* OwningPawn, AActor* TargetActor);
	
	UPROPERTY(EditAnywhere)
	float AnglePrecision;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
};
