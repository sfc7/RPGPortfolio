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

	INIT_TASK_NODE_NOTIFY_FLAGS();

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
}

void UBTTask_RotateToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
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
	FVector ForwardVector = OwningPawn->GetActorForwardVector();
	FVector NormalizedVector = (TargetActor->GetActorLocation() - OwningPawn->GetActorLocation()).GetSafeNormal();

	float DotResult = FVector::DotProduct(ForwardVector, NormalizedVector);
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);

	return Angle < AnglePrecision;
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	FRotateToTargetTaskInstance* CachedNodeMemory =CastInstanceNodeMemory<FRotateToTargetTaskInstance>(NodeMemory);
	check(CachedNodeMemory);

	CachedNodeMemory->OwningPawn = OwningPawn;
	CachedNodeMemory->TargetActor = TargetActor;

	if (!CachedNodeMemory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

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
