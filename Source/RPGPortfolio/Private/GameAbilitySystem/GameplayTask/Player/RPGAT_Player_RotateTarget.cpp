// LJS


#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"

URPGAT_Player_RotateTarget::URPGAT_Player_RotateTarget()
{
	bTickingTask = true;
}

void URPGAT_Player_RotateTarget::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnRotateTargetTaskTick.Broadcast(DeltaTime);
		if (bIsRotating)
		{
			CheckRotationComplete(DeltaTime);
		}
	}
	
	if (!bIsRotating) EndTask();
}

URPGAT_Player_RotateTarget* URPGAT_Player_RotateTarget::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	URPGAT_Player_RotateTarget* Node = NewAbilityTask<URPGAT_Player_RotateTarget>(OwningAbility);

	return Node;
}

void URPGAT_Player_RotateTarget::SetTargetRotation(FRotator NewTargetRotation)
{
	TargetRotation = NewTargetRotation;
	bIsRotating = true;
}

void URPGAT_Player_RotateTarget::CheckRotationComplete(float DeltaTime)
{
	AActor* AvatarActor = GetAvatarActor();
	if (!AvatarActor)
		return;
		
	APawn* AvatarPawn = Cast<APawn>(AvatarActor);
	if (!AvatarPawn)
		return;
	
	FRotator CurrentRotation = AvatarPawn->GetActorRotation();
	FRotator DeltaRotation = (TargetRotation - CurrentRotation).GetNormalized();
	
	if (FMath::Abs(DeltaRotation.Yaw) <= RotationTolerance)
	{
		bIsRotating = false;
		AvatarPawn->SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
		OnRotationCompleted.Broadcast();
		EndTask();
	}
}
