// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MW_UpdateTargetActor.generated.h"

class AMonsterCharacter;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTService_MW_UpdateTargetActor : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_MW_UpdateTargetActor();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
	
	AMonsterCharacter* MonsterCharacter;
};
