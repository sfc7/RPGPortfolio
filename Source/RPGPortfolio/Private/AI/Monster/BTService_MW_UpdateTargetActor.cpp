// LJS


#include "AI/Monster/BTService_MW_UpdateTargetActor.h"
#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MonsterCharacter.h"

UBTService_MW_UpdateTargetActor::UBTService_MW_UpdateTargetActor()
{
	NodeName = TEXT("MW_UpdateTargetActor");
}

void UBTService_MW_UpdateTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return;

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(Target)) return;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	// AttackTarget으로 WarpTarget 업데이트
	if (IsValid(MonsterCharacter))
	{
		MonsterCharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(FName("AttackTarget"), Target->GetActorLocation());
	}		
	else
	{
		MonsterCharacter = Cast<AMonsterCharacter>(AIController->GetPawn());
	}
}
