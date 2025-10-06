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
		// 회전 중인 경우 회전 완료 확인
		if (bIsRotating)
		{
			CheckRotationComplete(DeltaTime);
		}
	}
	
	if (!bIsRotating) EndTask();
}

URPGAT_Player_RotateTarget* URPGAT_Player_RotateTarget::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	// Tick 태스크 팩토리 생성 과정
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
	if (!IsValid(AvatarActor)) return;
		
	APawn* AvatarPawn = Cast<APawn>(AvatarActor);
	if (!IsValid(AvatarPawn)) return;
		
	// 현재 회전과 목표 회전 값 정규화
	FRotator CurrentRotation = AvatarPawn->GetActorRotation();
	FRotator DeltaRotation = (TargetRotation - CurrentRotation).GetNormalized();

	// 회전 허용 오차 내에 도달한 경우
	if (FMath::Abs(DeltaRotation.Yaw) <= RotationTolerance)
	{
		// 회전 상태 비활성화하고 회전 완료 로직 처리 후 Task 종료
		bIsRotating = false;
		AvatarPawn->SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
		OnRotationCompleted.Broadcast();
		EndTask();
	}
}
