// LJS


#include "RPGFunc.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "RPGStructTypes.h"
#include "ScalableFloat.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "Func/RPGCustomCountDownAction.h"
bool URPGFunc::CheckTargetTeamAgent(APawn* SourcePawn, APawn* TargetPawn)
{
	if (!IsValid(SourcePawn) || !IsValid(TargetPawn)) return false;

	// 팀 에이전트 인터페이스 캐스팅
	IGenericTeamAgentInterface* SourceTeamAgent = Cast<IGenericTeamAgentInterface>(SourcePawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	// 팀 에이전트 검사
	if (SourceTeamAgent && TargetTeamAgent)
	{
		return SourceTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float URPGFunc::GetScalableFloatconst(const FScalableFloat& ScalableFloat, float Level)
{
	return ScalableFloat.GetValueAtLevel(Level);
}

FGameplayTag URPGFunc::GetHitReactDirection(AActor* Attacker, AActor* HitReactActor, float& OutHitReactDirection)
{
	check(Attacker && HitReactActor);

	// 기본 벡터 계산
	const FVector ActorForward = HitReactActor->GetActorForwardVector();
	const FVector ActorLocation = HitReactActor->GetActorLocation();
	const FVector AttackerLocation = Attacker->GetActorLocation();
	const FVector ActorToAttackerNormalize = (AttackerLocation - ActorLocation).GetSafeNormal();

	// 내적 계산으로 각도 구하기
	const float InnerProduct = FVector::DotProduct(ActorForward, ActorToAttackerNormalize);
	OutHitReactDirection = UKismetMathLibrary::DegAcos(InnerProduct);

	// 외적 계산으로 방향 구하기
	const FVector CrossProduct = FVector::CrossProduct(ActorForward, ActorToAttackerNormalize);
	const float ZVectorInnerProduct = FVector::DotProduct(HitReactActor->GetActorUpVector(), CrossProduct);

	// 음수면 각도 반전
	if (ZVectorInnerProduct < 0.f)
	{
		OutHitReactDirection *= -1.f;
	}

	// 각도 범위에 따른 GameplayTag 반환
	if (OutHitReactDirection>=-45.f && OutHitReactDirection <=45.f)
	{
		return RPGGameplayTag::Character_Status_HitReact_Front;
	}
	else if (OutHitReactDirection<-135.f || OutHitReactDirection>135.f)
	{
		return RPGGameplayTag::Character_Status_HitReact_Back;
	}
	else if (OutHitReactDirection<-45.f && OutHitReactDirection>=-135.f)
	{
		return RPGGameplayTag::Character_Status_HitReact_Left;
	}
	else if(OutHitReactDirection>45.f && OutHitReactDirection<=135.f)
	{
		return RPGGameplayTag::Character_Status_HitReact_Right;
	}

	return RPGGameplayTag::Character_Status_HitReact_Front;
}

bool URPGFunc::IsValidDefense(AActor* Attacker, AActor* Defender)
{
	if (!IsValid(Attacker) && !IsValid(Defender)) return false;

	// 방어자가 공격자를 바라보는지 확인 (내적이 음수)
	const float InnerProduct = FVector::DotProduct(Attacker->GetActorForwardVector(), Defender->GetActorForwardVector());

	return InnerProduct < -0.1f;
}

void URPGFunc::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ERPGCountDownActionInput CountDownInput, ERPGCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (!IsValid(World)) return;

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	// 기존 액션 찾기
	FRPGCustomCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FRPGCustomCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	// 시작 입력 처리
	if (CountDownInput == ERPGCountDownActionInput::Start)
	{
		// 기존 액션이 없으면 새로 생성
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FRPGCustomCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
				);
		}		
	}

	// 취소 입력 처리
	if (CountDownInput == ERPGCountDownActionInput::Cancel)
	{
		// 기존 액션이 있으면 취소
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

UGeneralGameManager* URPGFunc::GetGeneralGameManager(const UObject* WorldContextObject)
{
	if (GEngine)
   {
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if (IsValid(World))
		{
			return GetGeneralGameManager(World);
		}
   }
	
	return nullptr;
}


