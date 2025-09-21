// LJS

#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTaskMonsterBase.h"
#include "BTTask_Teleport.generated.h"

/**
 * UBTTask_Teleport
 * 
 * 캐릭터 Teleport 사용에 대한 Task
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
