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
	if (!TargetActor) return;
	
	AActor* Owner = OwnerComp.GetOwner();
	if (!Owner) return;

	CachedOwnerComp = &OwnerComp;
	
	// Teleport는 AMonsterCharacter단에서 구현 되어 있음
	MonsterCharacter->Teleport(FVector::ZeroVector, TargetActor);
	// Teleport가 끝나는 시점에 Finish할 콜백함수를 넣음
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
