// LJS

#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTaskMonsterBase.h"
#include "BTTask_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTTask_Teleport : public UBTTaskMonsterBase
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UFUNCTION()
	void OnTeleportEnd();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
