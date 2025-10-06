	// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_LockOnTarget.generated.h"

class URPGWidgetBase;
class UInputMappingContext;

/**
 * URPGGA_Player_LockOnTarget
 * 
 * 플레이어용 락온 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_LockOnTarget : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_LockOnTarget();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//@ 타겟 락온 틱 업데이트
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	//@ 타겟 전환 (마우스 좌우로 전환)
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);
	
private:
	//@ 타겟 락온 시도
	void TryLockOnTarget();

	//@ 락온 가능한 액터들 탐지 함수
	void GetAvailableActorsToLock();

	//@ 락온 가능한 액터들 중 가장 가까운 타겟 선택
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	//@ 타겟 전환 용 현재 타겟 주변의 액터들 분류 
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);

	//@ 타겟 락온 위젯 그리기
	void DrawTargetLockWidget();
	
	//@ 타겟 락온 이동 설정 초기화
	void InitTargetLockMovement();

	//@ 타겟 락온 IMC 초기화
	void InitTargetLockMappingContext();

	//@ 타겟 락온 어빌리티 취소
	void CancelLockOnTargetAbility();

	//@ 락온 가능한 액터들 정리
	void CleanAvailableActors();

	//@ 타겟 락온 위젯 위치 설정
	void SetTargetLockWidgetPosition();

	//@ 타겟 락온 이동 설정 리셋
	void ResetTargetLockMovement();

	//@ 타겟 락온 IMC 리셋
	void ResetTargetLockMappingContext();

	//@ 락온 가능한 액터 체크할 정면방향 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float BoxTraceDistance = 5000.f;

	//@ 타겟 락온 시 최대 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float TargetLockMaxWalkSpeed = 150.f;

	//@ 타겟 락온 IMC
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* TargetLockMappingContext;
	
	//@ 타겟 락온 카메라 오프셋 거리
	UPROPERTY(EditDefaultsOnly)
	float TargetLockCameraOffsetDistance = 20.f;

	//@ 락온 가능한 액터 체크용 트레이스 박스의 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector TraceBoxSizeDistance = FVector(5000.f, 5000.f, 300.f);

	//@ 타겟 락온 회전 보간 속도
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float TargetLockRotationInterpSpeed = 5.f;

	//@ 박스 트레이스 채널
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	//@ 락온 디버깅 유무 플래그
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bShowPersistentDebugSphere = false;

	//@ 락온 가능한 액터들
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	//@ 현재 락온된 액터
	UPROPERTY()
	AActor* CurrentLockedActor;

	//@ 타겟 락온 위젯 클래스
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<URPGWidgetBase> TargetLockOnWidgetClass;

	//@ 타겟 락온 위젯 캐싱
	UPROPERTY()
	URPGWidgetBase* DrawTargetLockOnWidget;

	//@ 타겟 락온 위젯 크기
	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	//@ 캐시된 기본 최대 이동 속도
	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;

};
