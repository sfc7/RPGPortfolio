	// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_LockOnTarget.generated.h"

class URPGWidgetBase;
class UInputMappingContext;

/**
 * 
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

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);
	
private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);	
	void DrawTargetLockWidget();
	void InitTargetLockMovement();
	void InitTargetLockMappingContext();
	
	void CancelLockOnTargetAbility();
	void CleanAvailableActors();
	void SetTargetLockWidgetPosition();
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float TargetLockMaxWalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* TargetLockMappingContext;

	UPROPERTY(EditDefaultsOnly)
	float TargetLockCameraOffsetDistance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector TraceBoxSizeDistance = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	
	float TargetLockRotationInterpSpeed = 5.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bShowPersistentDebugSphere = false;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<URPGWidgetBase> TargetLockOnWidgetClass;

	UPROPERTY()
	URPGWidgetBase* DrawTargetLockOnWidget;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;

};
