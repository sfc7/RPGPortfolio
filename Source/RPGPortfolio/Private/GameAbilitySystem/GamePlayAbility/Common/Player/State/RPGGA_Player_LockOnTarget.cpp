// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_LockOnTarget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LandscapeGizmoActiveActor.h"
#include "Animation/AnimInstanceProxy.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/RPGWidgetBase.h"
#include "Controller/PlayerCharacterController.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

URPGGA_Player_LockOnTarget::URPGGA_Player_LockOnTarget()
{
}

void URPGGA_Player_LockOnTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	// 타겟 락온 시도, 필요한 속성 값들 초기화
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGGA_Player_LockOnTarget::EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	// 속성 값들 리셋, 캐싱된 액터 초기화
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanAvailableActors();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_LockOnTarget::OnTargetLockTick(float DeltaTime)
{
	// 현재 락온된 액터가 없는 경우 어빌리티 취소
	if (!IsValid(CurrentLockedActor))
	{
		CancelLockOnTargetAbility();
		return;
	}

	// 락온된 액터의 사망 상태 확인
	if (IsValid(CurrentLockedActor)) {
		URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentLockedActor));
		if (ASC)
		{
			if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead) || HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
			{
				CancelLockOnTargetAbility();
				return;
			}
		}
	}

	// 타겟 락온 위젯 위치 설정
	SetTargetLockWidgetPosition();

	// 회피 상태인지 디펜스 상태인지 체크
	const bool bShouldOverrideRotation =
		!HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDodging) &&
			!HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDefensing);

	// 락온 상태에서 회피 상태나 디펜스 상태에 따른 컨트롤러와 캐릭터 회전 처리 
	if (bShouldOverrideRotation)
	{
		if (IsValid(CurrentLockedActor))
		{
			FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		CurrentLockedActor->GetActorLocation());

			LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);
			
			const FRotator CurrentControlRot = GetPlayerControllerFromActorInfo()->GetControlRotation();
			const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

			GetPlayerControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
			GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
		}
	}
}

void URPGGA_Player_LockOnTarget::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	// 락온 가능한 액터들 탐지
	GetAvailableActorsToLock();

	// 좌우 액터 배열 초기화
	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	// 현재 타겟 주변의 액터들 분류
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	// 방향에 따른 새 타겟 선택
	if (InSwitchDirectionTag == RPGGameplayTag::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	// 새 타겟 설정
	if (IsValid(NewTargetToLock))
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void URPGGA_Player_LockOnTarget::TryLockOnTarget()
{
	// 락온 가능한 액터들 탐지
	GetAvailableActorsToLock();

	// 락온 가능한 액터가 없는 경우 어빌리티 취소 
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelLockOnTargetAbility();
		return;
	}

	// 가장 가까운 타겟 선택
	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	// 타겟 있으면 락온 그리고 없으면 어빌리티 취소 
	if (IsValid(CurrentLockedActor))
	{
		DrawTargetLockWidget();
		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelLockOnTargetAbility();
	}
}

void URPGGA_Player_LockOnTarget::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();
	
	TArray<FHitResult> BoxTraceHits;

	// 박스 트레이스 실행
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation() + GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSizeDistance / 2.f,
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugSphere? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		OUT BoxTraceHits,
		true
		);

	// 히트 결과에서 액터들 수집
	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != GetPlayerCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* URPGGA_Player_LockOnTarget::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	// 가장 가까운 액터 찾기
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void URPGGA_Player_LockOnTarget::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	// 현재 락온된 액터나 사용 가능한 액터가 없는 경우 어빌리티 취소
	if (!IsValid(CurrentLockedActor) || AvailableActorsToLock.IsEmpty())
	{
		CancelLockOnTargetAbility();
		return;
	}

	// 플레이어 위치와 현재 타겟 방향 벡터
	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	// 각 액터를 좌우로 분류
	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		// 유효하지 않거나 현재 타겟과 같은 경우 건너뛰기
		if (!IsValid(AvailableActor) || AvailableActor == CurrentLockedActor) continue;

		// 플레이어에서 액터로의 방향 벡터
		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		// 외적을 이용한 좌우 판단
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		// Z 성분이 양수면 오른쪽, 음수면 왼쪽으로 판단함
		if (CrossResult.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void URPGGA_Player_LockOnTarget::DrawTargetLockWidget()
{
	if (IsValid(DrawTargetLockOnWidget)) return;
	
	if (IsValid(TargetLockOnWidgetClass))
	{
		// 위젯 클래스 생성후 컨트롤러에 종속
		DrawTargetLockOnWidget = CreateWidget<URPGWidgetBase>(GetPlayerControllerFromActorInfo(), TargetLockOnWidgetClass);

		if (IsValid(DrawTargetLockOnWidget))
		{
			DrawTargetLockOnWidget->AddToViewport();
		}
	}
}

void URPGGA_Player_LockOnTarget::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;

	GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void URPGGA_Player_LockOnTarget::InitTargetLockMappingContext()
{
	const APlayerCharacterController* PlayerController = GetPlayerControllerFromActorInfo();
	if (!IsValid(PlayerController)) return;

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!IsValid(LocalPlayer)) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!IsValid(Subsystem)) return;

	// 타겟 락온 IMC 추가
	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void URPGGA_Player_LockOnTarget::ResetTargetLockMovement()
{
	// 캐시된 기본 최대 이동 속도가 유효한 경우 복원
	if (CachedDefaultMaxWalkSpeed>0.f)
	{
		GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;		
	}
}

void URPGGA_Player_LockOnTarget::ResetTargetLockMappingContext()
{
	const APlayerCharacterController* PlayerController = GetPlayerControllerFromActorInfo();
	if (!IsValid(PlayerController)) return;
	
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!IsValid(LocalPlayer)) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!IsValid(Subsystem)) return;

	// 타겟 락온 입력 매핑 컨텍스트 제거
	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void URPGGA_Player_LockOnTarget::CancelLockOnTargetAbility()
{
	// 락온 어빌리티 취소
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),GetCurrentActivationInfo(), true);
}

void URPGGA_Player_LockOnTarget::CleanAvailableActors()
{
	// 사용 가능한 액터들 정리
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	// 위젯 제거
	if (IsValid(DrawTargetLockOnWidget))
	{
		DrawTargetLockOnWidget->RemoveFromParent();
	}

	DrawTargetLockOnWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;

	// 캐싱된 속도 초기화
	CachedDefaultMaxWalkSpeed = 0.f;
}

void URPGGA_Player_LockOnTarget::SetTargetLockWidgetPosition()
{
	// 위젯이나 현재 락온된 액터가 없는 경우 어빌리티 취소 
	if (!IsValid(DrawTargetLockOnWidget) || !IsValid(CurrentLockedActor))
	{
		CancelLockOnTargetAbility();
		return;
	}

	// 월드 위치를 스크린 위치로 변환
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
		);

	// 위젯 크기가 설정되지 않은 경우 크기 계산
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawTargetLockOnWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}	
			}
		);
	} 

	// 위젯 위치 설정 (중앙 정렬)
	ScreenPosition -= (TargetLockWidgetSize/ 2.f);
	DrawTargetLockOnWidget->SetPositionInViewport(ScreenPosition, false);
}
