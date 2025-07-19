// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_Dodge.h"
#include "RPGFunc.h"
#include "MotionWarpingComponent.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/GameplayStatics.h"

URPGGA_Player_Dodge::URPGGA_Player_Dodge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CalcDodgeDirectionAndDistance();
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_Dodge"), DodgeMontage, 1.0f, NAME_None,
		true, 1.0f, false);
	
	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* DodgeSuccessGE = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, RPGGameplayTag::Player_Event_DodgeSuccess, nullptr, false, true
	);
	DodgeSuccessGE->EventReceived.AddDynamic(this, &URPGGA_Player_Dodge::SuccessDodgeCallback);
	DodgeSuccessGE->ReadyForActivation();
	
}

void URPGGA_Player_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetTimerManager().SetTimer(DodgeDelayTimerHandle, [this]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}, 0.1f, false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_Dodge::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_Dodge::CalcDodgeDirectionAndDistance()
{
	if (WarpTargetNameDirection == FName(TEXT("None")) || WarpTargetNameDirection == FName(TEXT(""))) return;
	
	DodgeDirection = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();
	
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		WarpTargetNameDirection, FVector::ZeroVector, FRotationMatrix::MakeFromX(DodgeDirection).Rotator());

	
	FVector ActorLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector DownVector = GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1.f;
	FVector StartLocation = ActorLocation + (DodgeDirection * (URPGFunc::GetScalableFloatconst(Distance, GetAbilityLevel())));
	FVector EndLocation = ActorLocation + (DownVector * 500.f);
	
	FHitResult FindGround;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;   
	QueryParams.AddIgnoredActor(GetPlayerCharacterFromActorInfo());
	
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		OUT FindGround, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);

	if (bHit)
	{
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(WarpTargetNameDistance, FindGround.ImpactPoint);
	}
	else
	{
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->RemoveWarpTarget(WarpTargetNameDistance);
	}
}

void URPGGA_Player_Dodge::SuccessDodgeCallback(FGameplayEventData PayloadData)
{
	FGameplayCueParameters DefenseParryingGCParam;
	DefenseParryingGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DodgeSuccessGamePlayCue, DefenseParryingGCParam);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraFade(0.0f, 0.25f, 0.1f, FLinearColor::Black, false, true);
		
		GetWorld()->GetTimerManager().SetTimer(FadeBackTimerHandle, [PC]()
		{
			PC->PlayerCameraManager->StartCameraFade(0.25f, 0.0f, 0.1f, FLinearColor::Black, false, true);
		}, 0.05f, false);
		
	}
	GetWorld()->GetTimerManager().SetTimer(DodgeDelayTimerHandle, [this]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}, 0.1f, false);
}	
