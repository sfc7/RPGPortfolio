// LJS


#include "AI/Monster/BTService_CheckMonsterDeath.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

UBTService_CheckMonsterDeath::UBTService_CheckMonsterDeath()
{
	NodeName = TEXT("CheckMonsterStatusDeath");
	Interval = 0.2f;
}

void UBTService_CheckMonsterDeath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* Owner = OwnerComp.GetOwner();
	if (!Owner) return;

	if (AAIController* AIController = Cast<AAIController>(Owner))
	{
		APawn* ControlledPawn = AIController->GetPawn();
		if (ControlledPawn)
		{
			URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn));
			if (ASC)
			{
				if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
				{
					BlackboardComp->SetValueAsBool(DeathStatusTag.SelectedKeyName, true);
				}
			}
		}
	}
}
