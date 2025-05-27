// LJS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGAIController : public AAIController
{
	GENERATED_BODY()
public:
	ARPGAIController(const FObjectInitializer& ObjectInitializer);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* MonsterAIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* MonsterAISenseConfig_Sight;

	UFUNCTION()
	virtual void OnMonsterPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UBehaviorTree* BehaviorTree;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CrowdFollowingComponent")
		bool bEnableDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category = "CrowdFollowingComponent", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
		int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "CrowdFollowingComponent", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
		float CollisionQueryRange = 600.f;
};
