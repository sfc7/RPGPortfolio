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
	if (!IsValid(BlackboardComp)) return false;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(TargetActor)) return false;

	AActor* OwnerActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(OwnerActor)) return false;

	URPGAbilitySystemComponent* ASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	if (!IsValid(ASC)) return false;

	// 플레이어와 AI 둘 다 Dead Tag 체크
	if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
	{
		bTargetActorDead = true;
	}
	
	if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
	{
		bOwnerActorDead = true;
	}
	
	const float DistToTarget = BlackboardComp->GetValueAsFloat(DistToTargetKey.SelectedKeyName);

	// 플레이어와 AI 둘 중 하나가 죽었거나 거리가 0인지 체크
	return (bTargetActorDead || bOwnerActorDead || FMath::IsNearlyZero(DistToTarget));
}
