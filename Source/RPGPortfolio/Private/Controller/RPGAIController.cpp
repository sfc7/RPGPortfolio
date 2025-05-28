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


	MonsterAISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("MonsterAISenseConfig_Sight");
	MonsterAISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	MonsterAISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	MonsterAISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	MonsterAISenseConfig_Sight->SightRadius = 5000.f; // 인식 거리
	MonsterAISenseConfig_Sight->LoseSightRadius = 0.f; // 적을 놓치지 않음
	MonsterAISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // 시야각도

	MonsterAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("MonsterAIPerceptionComponent");
	MonsterAIPerceptionComponent->ConfigureSense(*MonsterAISenseConfig_Sight);
	MonsterAIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	MonsterAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnMonsterPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type ARPGAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void ARPGAIController::BeginPlay()
{
	Super::BeginPlay();

	UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	if (CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default: break;
		}

		CrowdFollowingComponent->SetAvoidanceGroup(1);
		CrowdFollowingComponent->SetGroupsToAvoid(1);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void ARPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);

		ACharacter* InCharacter = Cast<ACharacter>(InPawn);
		if (IsValid(InCharacter))
		{
			UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsFloat(FName("MaxWalkSpeed"), InCharacter->GetCharacterMovement()->MaxWalkSpeed);
			}
		}	
	}
}

void ARPGAIController::OnMonsterPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"),Actor);
			}
		}
	}
	
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"),Actor);
		}
	}
}
