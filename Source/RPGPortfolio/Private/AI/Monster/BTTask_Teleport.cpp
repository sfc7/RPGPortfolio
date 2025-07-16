// LJS


#include "AI/Monster/BTTask_Teleport.h"

#include "Character/MonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_Teleport::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;
	
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	AActor* Owner = OwnerComp.GetOwner();
	if (!Owner) return;

	CachedOwnerComp = &OwnerComp;
	MonsterCharacter->Teleport(FVector::ZeroVector, TargetActor);
	MonsterCharacter->OnTeleportEnd.AddDynamic(this, &ThisClass::OnTeleportEnd);
}

void UBTTask_Teleport::OnTeleportEnd()
{
	if (MonsterCharacter)
	{
		MonsterCharacter->OnTeleportEnd.RemoveDynamic(this, &ThisClass::OnTeleportEnd);
	}
	
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
