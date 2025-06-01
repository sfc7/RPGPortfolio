// LJS


#include "AI/Monster/BTDeco_CheckGamePlayTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

bool UBTDeco_CheckGamePlayTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;
	
	AActor* OwnerActor = Cast<AActor>(BlackboardComp->GetValueAsObject(OwnerActorKey.SelectedKeyName));
	if (IsValid(OwnerActor))
	{
		URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor));
		if (ASC->HasMatchingGameplayTag(CheckGamePlayTag))
		{
			if (!bInverse) return true;
		}
		else
		{
			if (bInverse) return true;
		}
	}

	return false;
}
