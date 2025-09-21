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
	if (!IsValid(BlackboardComp)) return false;

	AActor* OwnerActor = Cast<AActor>(BlackboardComp->GetValueAsObject(OwnerActorKey.SelectedKeyName));
	if (!IsValid(OwnerActor)) return false;
	
	URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor));
	if (!IsValid(ASC)) return false;

	// bool 반전에 따라 Tag 체크
	if (ASC->HasMatchingGameplayTag(CheckGamePlayTag))
	{
		if (!bInverse) return true;
	}
	else
	{
		if (bInverse) return true;
	}
	

	return false;
}
