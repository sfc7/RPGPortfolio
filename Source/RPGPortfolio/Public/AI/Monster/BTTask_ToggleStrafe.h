// LJS

#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTaskMonsterBase.h"
#include "BTTask_ToggleStrafe.generated.h"

class URPGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTTask_ToggleStrafe : public UBTTaskMonsterBase
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector MaxWalkSpeedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ShouldEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ShouldChangeMaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float StrafeWalkSpeed;

	URPGAbilitySystemComponent* ASC;
};
