// LJS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMonsterBase.generated.h"

class AMonsterCharacter;
/**
* UBTTaskMonsterBase
 * 
 * 모든 Monster용 BTTask의 기본 구조, MonsterCharacter 캐싱, Tick시 ASC 상태 확인
 *    
 */
UCLASS()
class RPGPORTFOLIO_API UBTTaskMonsterBase : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskMonsterBase(FObjectInitializer const& ObjectInitializer);
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMonsterCharacter* MonsterCharacter;
};
