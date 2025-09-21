// LJS

#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTaskMonsterBase.h"
#include "GameplayTagContainer.h"
#include "BTTask_ActivateGameAbility.generated.h"

/**
 * UBTTask_ActivateGameAbility
 * 
 * Monster의 ASC에 접근해서 GameplayTag로 어빌리티 실행
 */
UCLASS()
class RPGPORTFOLIO_API UBTTask_ActivateGameAbility : public UBTTaskMonsterBase
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActivateGameplayTag;
};
