// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_CheckGamePlayTag.generated.h"

/**
 * UBTDeco_CheckGamePlayTag
 * 
 * Blackboard에서 가져온 Actor가 특정 GameplayTag를 가지고 있는지 체크
 */
UCLASS()
class RPGPORTFOLIO_API UBTDeco_CheckGamePlayTag : public UBTDecorator
{
	GENERATED_BODY()
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector OwnerActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayTag CheckGamePlayTag;

	// return bool 값 반전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bInverse;
};
