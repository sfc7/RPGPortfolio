// LJS


#include "AI/Monster/BTTaskMonsterBase.h"
#include "AIController.h"
#include "Character/MonsterCharacter.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

UBTTaskMonsterBase::UBTTaskMonsterBase(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskMonsterBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result) return Result;
	
	MonsterCharacter = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(MonsterCharacter)) return EBTNodeResult::Failed;
	
	OnExecuteTask(OwnerComp, NodeMemory);
	
	return Result = EBTNodeResult::InProgress;
}

void UBTTaskMonsterBase::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
}

void UBTTaskMonsterBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	URPGAbilitySystemComponent* ASC = MonsterCharacter->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return;
	
	if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_IsTasking))
	{
		return;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
	