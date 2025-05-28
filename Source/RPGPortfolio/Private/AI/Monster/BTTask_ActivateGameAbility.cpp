// LJS


#include "AI/Monster/BTTask_ActivateGameAbility.h"
#include "Character/MonsterCharacter.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

EBTNodeResult::Type UBTTask_ActivateGameAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_ActivateGameAbility::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnExecuteTask(OwnerComp, NodeMemory);

	URPGAbilitySystemComponent* ASC = MonsterCharacter->GetRPGAbilitySystemComponent();
	ASC->ActivateGamePlayAbilityByTag(ActivateGameplayTag);
}
