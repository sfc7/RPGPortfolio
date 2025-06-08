// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_Dodge.h"
#include "RPGFunc.h"
#include "MotionWarpingComponent.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

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
	
}

void URPGGA_Player_Dodge::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_Dodge::CalcDodgeDirectionAndDistance()
{
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
