// LJS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGAIController.generated.h"

// 군중 회피에서  CrowdAvoidanceQuality용 열거형
UENUM(BlueprintType)
enum class ECrowdAvoidanceQualityLevel : uint8
{
	Low     = 0,
	Medium  = 1,
	Good    = 2,
	High    = 3
};

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;
/**
 * ARPGAIController
 * 
 * Monster AI를 위한 AIController, AIPerception, CrowdFollowingComponent등 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGAIController : public AAIController
{
	GENERATED_BODY()
public:
	ARPGAIController(const FObjectInitializer& ObjectInitializer);

	//@ 다른 액터에 대한 팀 태도 반환
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	//@ 몬스터 AI Perception
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* MonsterAIPerceptionComponent;

	//@ AI Perception중 시야
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* MonsterAISenseConfig_Sight;

	//@ 몬스터 퍼셉션 업데이트 콜백 - 타겟 감지 처리용
	UFUNCTION()
	virtual void OnMonsterPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UBehaviorTree* BehaviorTree;

private:
	//@ 군중 회피 시스템 활성화 여부
	UPROPERTY(EditDefaultsOnly, Category = "CrowdFollowingComponent")
		bool bEnableDetourCrowdAvoidance = true;

	
	//@ 군중 회피 레벨
	UPROPERTY(EditDefaultsOnly, Category = "CrowdFollowingComponent", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	ECrowdAvoidanceQualityLevel CrowdAvoidanceQualityLevel = ECrowdAvoidanceQualityLevel::Medium;
	
	//@ 군중 회피 컴포넌트용 충돌 쿼리 범위
	UPROPERTY(EditDefaultsOnly, Category = "CrowdFollowingComponent", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
		float CollisionQueryRange = 600.f;
};
