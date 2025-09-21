// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckMonsterDeath.generated.h"

/**
 * UBTService_CheckMonsterDeath
 * 
 * 일정 간격으로 몬스터 사망 상태 체크
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
