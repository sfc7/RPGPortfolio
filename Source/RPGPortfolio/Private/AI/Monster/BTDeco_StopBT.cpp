// LJS


#include "AI/Monster/BTDeco_StopBT.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

bool UBTDeco_StopBT::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (TargetActor)
	{
		URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
		if (ASC)
		{
			if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
			{
				bTargetActorDead = true;
			}
		}
	}

	AActor* OwnerActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (OwnerActor)
	{
		URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor));
		if (ASC)
		{
			if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
			{
				bOwnerActorDead = true;
			}
		}
	}
	
	float DistToTarget = BlackboardComp->GetValueAsFloat(DistToTargetKey.SelectedKeyName);
	
	return (bTargetActorDead || bOwnerActorDead || FMath::IsNearlyZero(DistToTarget));
}
