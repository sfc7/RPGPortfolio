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
	if (IsValid(SourcePawn) && IsValid(TargetPawn))
	{
		IGenericTeamAgentInterface* SourceTeamAgent = Cast<IGenericTeamAgentInterface>(SourcePawn->GetController());
		IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

		if (SourceTeamAgent && TargetTeamAgent)
		{
			return SourceTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
		}
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

	const FVector ActorForward = HitReactActor->GetActorForwardVector();
	const FVector ActorLocation = HitReactActor->GetActorLocation();
	const FVector AttackerLocation = Attacker->GetActorLocation();
	const FVector ActorToAttackerNormalize = (AttackerLocation - ActorLocation).GetSafeNormal();

	const float InnerProduct = FVector::DotProduct(ActorForward, ActorToAttackerNormalize);
	OutHitReactDirection = UKismetMathLibrary::DegAcos(InnerProduct);

	const FVector CrossProduct = FVector::CrossProduct(ActorForward, ActorToAttackerNormalize);
	const float ZVectorInnerProduct = FVector::DotProduct(HitReactActor->GetActorUpVector(), CrossProduct);

	if (ZVectorInnerProduct < 0.f)
	{
		OutHitReactDirection *= -1.f;
	}
	
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
	check(Attacker && Defender);

	const float InnerProduct = FVector::DotProduct(Attacker->GetActorForwardVector(), Defender->GetActorForwardVector());

	return InnerProduct < -0.1f;
	
}

bool URPGFunc::ApplyGameplayEffectSpecHandleToTargetActor(AActor* Instigator, AActor* TargetActor, const FGameplayEffectSpecHandle& SpecHandle)
{
	URPGAbilitySystemComponent* InstigatorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator));
	URPGAbilitySystemComponent* TargetActorASC = CastChecked<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetActorASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void URPGFunc::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ERPGCountDownActionInput CountDownInput, ERPGCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (!World) return;

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	FRPGCustomCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FRPGCustomCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	if (CountDownInput == ERPGCountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FRPGCustomCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
				);
		}		
	}

	if (CountDownInput == ERPGCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
			
		}
	}
}


