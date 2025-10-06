// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RPGAT_Player_RotateTarget.generated.h"

//@ 회전 타겟 틱 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotateTargetTickDelegate,float,DeltaTime);

//@ 회전 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationCompletedDelegate); 
/**
 * URPGAT_Player_RotateTarget
 * 
 * 플레이어가 타겟으로 회전하는 AbilityTask
 */
UCLASS()
class RPGPORTFOLIO_API URPGAT_Player_RotateTarget : public UAbilityTask
{
	GENERATED_BODY()
public:
	URPGAT_Player_RotateTarget();
	
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static URPGAT_Player_RotateTarget* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);
	
	//@ 회전 완료 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnRotationCompletedDelegate OnRotationCompleted;

	//@ 회전 타겟 틱 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnRotateTargetTickDelegate OnRotateTargetTaskTick;

	//@ 목표 회전값
	FRotator TargetRotation;
	//@ 회전 중 여부
	bool bIsRotating = false;
	//@ 회전 허용 오차 (도)
	float RotationTolerance = 5.0f;

	//@ 목표 회전 값 설정
	UFUNCTION(BlueprintCallable)
	void SetTargetRotation(FRotator NewTargetRotation);

	//@ 회전 완료 확인
	UFUNCTION()
	void CheckRotationComplete(float DeltaTime);
};
