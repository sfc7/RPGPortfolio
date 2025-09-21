// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToTarget.generated.h"

// 여러 AI가 동시에 BTTaskNode를 실행할 수 있어서 만든 인스턴스별 임시 메모리 저장 구조체
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
 * UBTTask_RotateToTarget
 * 
 * 타겟 Actor를 바라보도록 회전
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

	// 현재 회전이 목표 각도에 도달했는지 확인
	bool HasReachedAnglePercision(APawn* OwningPawn, AActor* TargetActor);
	
	UPROPERTY(EditAnywhere)
	float AnglePrecision;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
};
