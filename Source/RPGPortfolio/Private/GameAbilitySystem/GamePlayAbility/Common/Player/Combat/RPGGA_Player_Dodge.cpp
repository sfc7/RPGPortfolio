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

	//@ 회피 방향 및 거리 계산
	CalcDodgeDirectionAndDistance();
	
	// 회피 Montage 실행 및 바인딩
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_Dodge"), DodgeMontage, 1.0f, NAME_None,
		true, 1.0f, false);
	
	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Dodge::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 회피 성공 이펙트 관련 콜백 처리
	UAbilityTask_WaitGameplayEvent* DodgeSuccessGE = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, RPGGameplayTag::Player_Event_DodgeSuccess, nullptr, false, true
	);
	DodgeSuccessGE->EventReceived.AddDynamic(this, &URPGGA_Player_Dodge::SuccessDodgeCallback);
	DodgeSuccessGE->ReadyForActivation();
	
}

void URPGGA_Player_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 글로벌 시간 지연 복원
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

	// 모션 워핑으로 방향 적용
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		WarpTargetNameDirection, FVector::ZeroVector, FRotationMatrix::MakeFromX(DodgeDirection).Rotator());

	// 회피가 가능한 위치인지 지면 검사
	const FVector ActorLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector DownVector = GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1.f;
	const FVector StartLocation = ActorLocation + (DodgeDirection * (URPGFunc::GetScalableFloatconst(Distance, GetAbilityLevel())));
	const FVector EndLocation = ActorLocation + (DownVector * 500.f);
	
	FHitResult FindGround;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;   
	QueryParams.AddIgnoredActor(GetPlayerCharacterFromActorInfo());
	
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		OUT FindGround, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);

	// 지면 검사 성공 시 모션 워핑으로 거리 적용
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
	// 회피 성공 이펙트 GameplayCue 실행, , 
	FGameplayCueParameters DefenseParryingGCParam;
	DefenseParryingGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(DodgeSuccessGamePlayCue, DefenseParryingGCParam);
	// 글로벌 시간 지연
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
	
	// 카메라 페이드 적용
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
