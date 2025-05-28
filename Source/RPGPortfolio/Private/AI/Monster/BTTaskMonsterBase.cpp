// LJS


#include "AI/Monster/BTTaskMonsterBase.h"
#include "AIController.h"
#include "Character/MonsterCharacter.h"


EBTNodeResult::Type UBTTaskMonsterBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result) {
		return Result;
	}

	MonsterCharacter = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (IsValid(MonsterCharacter))
	{
		OnExecuteTask(OwnerComp, NodeMemory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return Result = EBTNodeResult::Succeeded;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	return Result = EBTNodeResult::Failed;
}

void UBTTaskMonsterBase::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
}
	