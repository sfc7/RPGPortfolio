// LJS


#include "AI/Monster/BTTask_RotateToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTService.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
	NodeName = TEXT("RotateToTarget");
	AnglePrecision = 10.f;
	RotationSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	// Notify 플래그 초기화
	INIT_TASK_NODE_NOTIFY_FLAGS();

	//BT 에디터에서 TargetActorKey를 Actor 타입만 선택 가능하게
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
}

void UBTTask_RotateToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		// TickTask에서 쓸 Key ID 가져오기
		TargetActorKey.ResolveSelectedKey(*BlackboardData);
	}
}

uint16 UBTTask_RotateToTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToTargetTaskInstance);
}

FString UBTTask_RotateToTarget::GetStaticDescription() const
{
	const FString KeyDescription = TargetActorKey.SelectedKeyName.ToString();
	
	return Super::GetStaticDescription();
}

bool UBTTask_RotateToTarget::HasReachedAnglePercision(APawn* OwningPawn, AActor* TargetActor)
{
	const FVector ForwardVector = OwningPawn->GetActorForwardVector();
	const FVector NormalizedVector = (TargetActor->GetActorLocation() - OwningPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(ForwardVector, NormalizedVector);
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);

	return Angle < AnglePrecision;
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!IsValid(ActorObject)) return EBTNodeResult::Failed;
	
	AActor* TargetActor = Cast<AActor>(ActorObject);
	if (!IsValid(TargetActor)) return EBTNodeResult::Failed;
	
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(OwningPawn)) return EBTNodeResult::Failed;
	
	FRotateToTargetTaskInstance* CachedNodeMemory =CastInstanceNodeMemory<FRotateToTargetTaskInstance>(NodeMemory);
	check(CachedNodeMemory);

	CachedNodeMemory->OwningPawn = OwningPawn;
	CachedNodeMemory->TargetActor = TargetActor;

	if (!CachedNodeMemory->IsValid()) return EBTNodeResult::Failed;
	
	// 현재 회전이 목표 각도에 도달했는지 확인 후 임시 메모리는 리셋하고 Task 성공 리턴
	if (HasReachedAnglePercision(OwningPawn, TargetActor))
	{
		CachedNodeMemory->Reset();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToTargetTaskInstance* CachedNodeMemory = CastInstanceNodeMemory<FRotateToTargetTaskInstance>(NodeMemory);

	if (!CachedNodeMemory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	// 현재 회전이 목표 각도에 도달했는지 확인 후 임시 메모리는 리셋하고 Task Finish, 안했으면 회전 보간
	if (HasReachedAnglePercision(CachedNodeMemory->OwningPawn.Get(), CachedNodeMemory->TargetActor.Get()))
	{
		CachedNodeMemory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CachedNodeMemory->OwningPawn->GetActorLocation(), CachedNodeMemory->TargetActor.Get()->GetActorLocation());
		FRotator TargetRotation = FMath::RInterpTo(CachedNodeMemory->OwningPawn->GetActorRotation(), LookRotation, DeltaSeconds, RotationSpeed);

		CachedNodeMemory->OwningPawn->SetActorRotation(TargetRotation);
	}
}
