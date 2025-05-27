// LJS


#include "AI/Monster/BTService_DistanceFromTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DistanceFromTarget::UBTService_DistanceFromTarget()
{
	NodeName = TEXT("DistanceFromTarget");
	Interval = 0.2f;
}

void UBTService_DistanceFromTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	float CalcDistance = AIController->GetPawn()->GetDistanceTo(Target);
	BlackboardComp->SetValueAsFloat(DistanceFromTarget.SelectedKeyName, CalcDistance);
}
