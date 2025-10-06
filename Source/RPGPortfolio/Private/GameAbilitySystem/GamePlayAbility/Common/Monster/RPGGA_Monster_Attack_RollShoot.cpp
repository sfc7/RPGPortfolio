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

	// 구르고 사격하는 패턴 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
this,TEXT("Player_Attack_RollShoot"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_RollShoot::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 총의 발사체 생성
	UAbilityTask_WaitGameplayEvent* WaitSpawnProjectile = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
	);

	WaitSpawnProjectile->EventReceived.AddDynamic(this, &ThisClass::SpawnBullet);
	WaitSpawnProjectile->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 사격 시 무기를 보여주는 연출용
	UAbilityTask_WaitGameplayEvent* WaitShowWeapon = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_ShowWeapon, nullptr, false, true
);

	WaitShowWeapon->EventReceived.AddDynamic(this, &ThisClass::ShowWeapon);
	WaitShowWeapon->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 사격 시 무기를 숨기 연출용
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
	// 사격 패턴에 사용할 BoxTrace 를 위한 초기 값
	const FVector StartLocation = GetMonsterCharacterFromActorInfo()->GetActorLocation();
	const FVector EndLocation = GetMonsterCharacterFromActorInfo()->GetActorForwardVector() * 100.f;
	const FRotator EndRotation = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	// 사격 패턴을 적용할 오브젝트 타입설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;

	// 데미지를 적용할 적중한 HitResult들
	TArray<FHitResult> OUTHitResults;
	// 사격 패턴에 대상 감지에 사용할 BoxTrace 생성
	UKismetSystemLibrary::BoxTraceMultiForObjects(this, StartLocation, EndLocation, FVector(BulletTraceRadius, BulletTraceRadius, 200.f), EndRotation,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OUTHitResults, true,FColor::Green, FColor::Red, 2.0f);

	// 타겟 지정
	AActor* TargetActor = nullptr;

	for (FHitResult OUTHitResult : OUTHitResults)
	{
		if (Cast<APlayerCharacterBase>(OUTHitResult.GetActor()))
		{
			TargetActor = OUTHitResult.GetActor();
		}
	}
	// 발사체가 생성되는 위치
	const FVector ProjectileLocation = GetOwningComponentFromActorInfo()->GetSocketLocation(BulletMuzzle);
	const FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetMonsterCharacterFromActorInfo();
	SpawnParams.Instigator = GetMonsterCharacterFromActorInfo();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 발사체 생성
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileLocation, ProjectileDirection, SpawnParams);
	Projectile->LaunchProjectile((TargetActor->GetActorLocation() - ProjectileLocation).GetSafeNormal(), 1500.0f);
	Projectile->GetProjectileMovementComponent()->bIsHomingProjectile = true;
	Projectile->GetProjectileMovementComponent()->HomingAccelerationMagnitude = 2000.f;
	Projectile->GetProjectileMovementComponent()->HomingTargetComponent = TargetActor->GetRootComponent();
	Projectile->SetIsUse(true);

	// 발사체가 충돌하면 데미지를 처리할 GameplayEffectSpecHandle
	Projectile->DamageEffectSpecHandle = MakeMonsterBaseDamageEffectSpecHandle(DamageEffectClass, DamageScale);
}

void URPGGA_Monster_Attack_RollShoot::ShowWeapon(FGameplayEventData PayloadData)
{
	// 파라곤 에셋의 문제로 무기를 보여주는 것을 AnimInstance로 처리함
	UCharacterAnimInstance* CharacterAnim = Cast<UCharacterAnimInstance>(CurrentActorInfo->GetAnimInstance());
	if (CharacterAnim)
	{
		CharacterAnim->SetbFirstUnEquipWeapon(true);
	}
}

void URPGGA_Monster_Attack_RollShoot::HideWeapon(FGameplayEventData PayloadData)
{
	// 파라곤 에셋의 문제로 무기를 숨기는 것을 AnimInstance로 처리함
	UCharacterAnimInstance* CharacterAnim = Cast<UCharacterAnimInstance>(CurrentActorInfo->GetAnimInstance());
	if (CharacterAnim)
	{
		CharacterAnim->SetbFirstUnEquipWeapon(false);
	}
}
