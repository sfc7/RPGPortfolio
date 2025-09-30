// LJS


#include "Controller/RPGAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ARPGAIController::ARPGAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// AI Perception 시야 감지 설정
	MonsterAISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("MonsterAISenseConfig_Sight");
	MonsterAISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	MonsterAISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	MonsterAISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	MonsterAISenseConfig_Sight->SightRadius = 5000.f; // 인식 거리
	MonsterAISenseConfig_Sight->LoseSightRadius = 0.f; // 적을 놓치지 않음
	MonsterAISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // 시야각도

	// AI Perception 감지 설정
	MonsterAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("MonsterAIPerceptionComponent");
	MonsterAIPerceptionComponent->ConfigureSense(*MonsterAISenseConfig_Sight);
	MonsterAIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	MonsterAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnMonsterPerceptionUpdated);

	// 1번이 몬스터
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type ARPGAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	if (!IsValid(OtherPawn)) return ETeamAttitude::Neutral;

	// 다른 폰의 컨트롤러에서 팀 에이전트 인터페이스 가져오기
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	// 팀 ID 비교하여 적대 관계 결정
	const FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
	const FGenericTeamId MyTeamId = GetGenericTeamId();
	
	// 다른 팀 ID가 낮으면 적대 (플레이어는 보통 0번 팀)
	if (OtherTeamId < MyTeamId)
	{
		return ETeamAttitude::Hostile;
	}

	// 같은 팀이거나 높은 팀 ID면 우호
	return ETeamAttitude::Friendly;
}

void ARPGAIController::BeginPlay()
{
    Super::BeginPlay();

	// 군중 회피 컴포넌트 설정
	UCrowdFollowingComponent* const CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	if (!IsValid(CrowdFollowingComponent)) return;

	// 군중 회피 상태 설정
	const ECrowdSimulationState SimulationState = bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled;
	CrowdFollowingComponent->SetCrowdSimulationState(SimulationState);

	// 군중 회피 레벨 설정
	switch (CrowdAvoidanceQualityLevel)
	{
		case ECrowdAvoidanceQualityLevel::Low: 
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); 
			break;
		case ECrowdAvoidanceQualityLevel::Medium: 
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); 
			break;
		case ECrowdAvoidanceQualityLevel::Good: 
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); 
			break;
		case ECrowdAvoidanceQualityLevel::High: 
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); 
			break;
		default: 
			break;
	}

	// 회피 그룹 설정
	CrowdFollowingComponent->SetAvoidanceGroup(1);
	CrowdFollowingComponent->SetGroupsToAvoid(1);
	CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
}

void ARPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(BehaviorTree)) return;
	
	RunBehaviorTree(BehaviorTree);
	
	const ACharacter* InCharacter = Cast<ACharacter>(InPawn);
	if (!IsValid(InCharacter)) return;

	UBlackboardComponent* const BlackboardComp = GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return;

	// 캐릭터의 최대 이동 속도를 블랙보드에 저장
	const float MaxWalkSpeed = InCharacter->GetCharacterMovement()->MaxWalkSpeed;
	BlackboardComp->SetValueAsFloat(FName("MaxWalkSpeed"), MaxWalkSpeed);
}

void ARPGAIController::OnMonsterPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* const BlackboardComponent = GetBlackboardComponent();
	if (!IsValid(BlackboardComponent)) return;

	if (!IsValid(Actor)) return;

	// 성공적으로 감지된 경우에만 처리 
	if (!Stimulus.WasSuccessfullySensed()) return;

	// 현재 타겟이 없거나 새로운 타겟으로 업데이트
	const AActor* CurrentTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("TargetActor")));
	if (!IsValid(CurrentTarget) || CurrentTarget != Actor)
	{
		// 새로운 타겟을 블랙보드에 설정
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
	}
}
