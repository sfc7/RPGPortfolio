// LJS


#include "AI/Monster/BTService_RotateToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_RotateToTarget::UBTService_RotateToTarget()
{
	NodeName = TEXT("RotateToTarget");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationSpeed = 5.f;
	Interval = 0.f;
	RandomDeviation = 0.f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
}

void UBTService_RotateToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BlackboardData);
	}
}

void UBTService_RotateToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (AIController)
	{
		APawn* OwnerPawn = AIController->GetPawn();
		if (OwnerPawn && Target)
		{
			FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), Target->GetActorLocation());
			FRotator InterpRotation = FMath::RInterpTo(OwnerPawn->GetActorRotation(), LookRotation, DeltaSeconds, RotationSpeed);

			OwnerPawn->SetActorRotation(InterpRotation);
		}
	}
}

FString UBTService_RotateToTarget::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}
	