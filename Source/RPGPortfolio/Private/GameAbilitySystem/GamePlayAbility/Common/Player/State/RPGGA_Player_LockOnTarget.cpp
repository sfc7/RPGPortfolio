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
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGGA_Player_LockOnTarget::EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanAvailableActors();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_LockOnTarget::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor)
	{
		CancelLockOnTargetAbility();
		return;
	}
	
	if (CurrentLockedActor) {
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
	
	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation =
		!HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDodging) &&
			!HasMatchingGameplayTag(RPGGameplayTag::Player_Status_ActionState_IsDefensing);
	
	if (bShouldOverrideRotation)
	{
		if (CurrentLockedActor)
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
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;
	
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == RPGGameplayTag::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	} else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void URPGGA_Player_LockOnTarget::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelLockOnTargetAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
	if (CurrentLockedActor)
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
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void URPGGA_Player_LockOnTarget::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelLockOnTargetAbility();
		return;
	}

	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

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
	if (!DrawTargetLockOnWidget)
	{
		if (TargetLockOnWidgetClass)
		{
			DrawTargetLockOnWidget = CreateWidget<URPGWidgetBase>(GetPlayerControllerFromActorInfo(), TargetLockOnWidgetClass);

			if (DrawTargetLockOnWidget)
			{
				DrawTargetLockOnWidget->AddToViewport();
			}
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
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void URPGGA_Player_LockOnTarget::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed>0.f)
	{
		GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void URPGGA_Player_LockOnTarget::ResetTargetLockMappingContext()
{
	if (!GetPlayerControllerFromActorInfo())
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void URPGGA_Player_LockOnTarget::CancelLockOnTargetAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),GetCurrentActivationInfo(), true);
}

void URPGGA_Player_LockOnTarget::CleanAvailableActors()
{
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	if (DrawTargetLockOnWidget)
	{
		DrawTargetLockOnWidget->RemoveFromParent();
	}

	DrawTargetLockOnWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}

void URPGGA_Player_LockOnTarget::SetTargetLockWidgetPosition()
{
	if (!DrawTargetLockOnWidget || !CurrentLockedActor)
	{
		CancelLockOnTargetAbility();
		return;
	}

	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
		);

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

	ScreenPosition -= (TargetLockWidgetSize/ 2.f);
	DrawTargetLockOnWidget->SetPositionInViewport(ScreenPosition, false);
}
