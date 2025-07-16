// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_RollShoot.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MeshPaintVisualize.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/MonsterCharacter.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WorldStatic/Projectile/ProjectileBase.h"
#include "Character/MonsterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/CharacterAnimInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

URPGGA_Monster_Attack_RollShoot::URPGGA_Monster_Attack_RollShoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_RollShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
this,TEXT("Player_Attack_RollShoot"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitSpawnProjectile = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
	);

	WaitSpawnProjectile->EventReceived.AddDynamic(this, &ThisClass::SpawnBullet);
	WaitSpawnProjectile->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitShowWeapon = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_ShowWeapon, nullptr, false, true
);

	WaitShowWeapon->EventReceived.AddDynamic(this, &ThisClass::ShowWeapon);
	WaitShowWeapon->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitHideWeapon = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_HideWeapon, nullptr, false, true
	);

	WaitHideWeapon->EventReceived.AddDynamic(this, &ThisClass::HideWeapon);
	WaitHideWeapon->ReadyForActivation();
}

void URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Monster_Attack_RollShoot::SpawnBullet(FGameplayEventData PayloadData)
{

	FVector StartLocation = GetMonsterCharacterFromActorInfo()->GetActorLocation();
	FVector EndLocation = GetMonsterCharacterFromActorInfo()->GetActorForwardVector() * 100.f;
	FRotator EndRotation = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	
	TArray<FHitResult> OUTHitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(this, StartLocation, EndLocation, FVector(BulletTraceRadius, BulletTraceRadius, 200.f), EndRotation,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OUTHitResults, true,FColor::Green, FColor::Red, 2.0f);

	AActor* TargetActor = nullptr;

	for (FHitResult OUTHitResult : OUTHitResults)
	{
		if (Cast<APlayerCharacterBase>(OUTHitResult.GetActor()))
		{
			TargetActor = OUTHitResult.GetActor();
		}
	}
	FVector ProjectileLocation = GetOwningComponentFromActorInfo()->GetSocketLocation(BulletMuzzle);
	FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetMonsterCharacterFromActorInfo();
	SpawnParams.Instigator = GetMonsterCharacterFromActorInfo();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileLocation, ProjectileDirection, SpawnParams);
	Projectile->LaunchProjectile((TargetActor->GetActorLocation() - ProjectileLocation).GetSafeNormal(), 1500.0f);
	Projectile->GetProjectileMovementComponent()->bIsHomingProjectile = true;
	Projectile->GetProjectileMovementComponent()->HomingAccelerationMagnitude = 2000.f;
	Projectile->GetProjectileMovementComponent()->HomingTargetComponent = TargetActor->GetRootComponent();
	Projectile->SetIsUse(true);
	
	Projectile->DamageEffectSpecHandle = MakeMonsterBaseDamageEffectSpecHandle(DamageEffectClass, DamageScale);
}

void URPGGA_Monster_Attack_RollShoot::ShowWeapon(FGameplayEventData PayloadData)
{
	
	UCharacterAnimInstance* CharacterAnim = Cast<UCharacterAnimInstance>(CurrentActorInfo->GetAnimInstance());
	if (CharacterAnim)
	{
		CharacterAnim->SetbFirstUnEquipWeapon(true);
	}
}

void URPGGA_Monster_Attack_RollShoot::HideWeapon(FGameplayEventData PayloadData)
{
	UCharacterAnimInstance* CharacterAnim = Cast<UCharacterAnimInstance>(CurrentActorInfo->GetAnimInstance());
	if (CharacterAnim)
	{
		CharacterAnim->SetbFirstUnEquipWeapon(false);
	}
}
