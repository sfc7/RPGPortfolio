// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckMonsterDeath.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTService_CheckMonsterDeath : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckMonsterDeath();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DeathStatusTag;
};
