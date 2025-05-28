// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMonsterBase.generated.h"

class AMonsterCharacter;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTTaskMonsterBase : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMonsterCharacter* MonsterCharacter;
};
