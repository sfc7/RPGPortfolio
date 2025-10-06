// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_Meteor.h"

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
#include "WorldStatic/AOE/AOEBase.h"
#include "Character/MonsterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/CharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"

URPGGA_Monster_Attack_Meteor::URPGGA_Monster_Attack_Meteor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_Meteor::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 메테오 패턴 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
this,TEXT("Player_Attack_Meteor"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_Meteor::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_Meteor::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_Meteor::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_Meteor::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 메테오 생성
	UAbilityTask_WaitGameplayEvent* WaitSpawnProjectile = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
	);

	WaitSpawnProjectile->EventReceived.AddDynamic(this, &ThisClass::SpawnMeteor);
	WaitSpawnProjectile->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 메테오 사용 시 무기를 보여주는 연출용
	UAbilityTask_WaitGameplayEvent* WaitShowWeapon = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_ShowWeapon, nullptr, false, true
	);


	WaitShowWeapon->EventReceived.AddDynamic(this, &ThisClass::ShowWeapon);
	WaitShowWeapon->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 메테오 종료 시 무기를 숨기 연출용
	UAbilityTask_WaitGameplayEvent* WaitHideWeapon = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, RPGGameplayTag::Character_Event_HideWeapon, nullptr, false, true
	);

	WaitHideWeapon->EventReceived.AddDynamic(this, &ThisClass::HideWeapon);
	WaitHideWeapon->ReadyForActivation();
}

void URPGGA_Monster_Attack_Meteor::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Monster_Attack_Meteor::SpawnMeteor(FGameplayEventData PayloadData)
{
	// 메테오 패턴에 사용할 BoxTrace 를 위한 초기 값
	const FVector StartLocation = GetMonsterCharacterFromActorInfo()->GetActorLocation();
	const FVector EndLocation = GetMonsterCharacterFromActorInfo()->GetActorForwardVector() * 100.f;
	const FRotator EndRotation = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	// 메테오 패턴을 적용할 오브젝트 타입설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;

	// 데미지를 적용할 적중한 HitResult들
	TArray<FHitResult> OUTHitResults;
	// 지진 패턴에 대상 감지에 사용할 BoxTrace 생성
	UKismetSystemLibrary::BoxTraceMultiForObjects(this, StartLocation, EndLocation, FVector(MeteorTraceRadius, MeteorTraceRadius, 200.f), EndRotation,
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

	// 메테오가 떨어지는 위치용 데칼
	const FVector DecalLocation = TargetActor->GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetMonsterCharacterFromActorInfo();
	SpawnParams.Instigator = GetMonsterCharacterFromActorInfo();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const AAOEBase* SpawnDecal = GetWorld()->SpawnActor<AAOEBase>(MeteorDecalClass, DecalLocation, FRotator::ZeroRotator,SpawnParams);

	// 메테오가 생성되는 랜덤 위치
	const float SignRandomOffset = FMath::RandBool() ? MeteorRandomOffset : -MeteorRandomOffset;
	
	const FVector RandomOffset = FVector(
	SignRandomOffset, 
	SignRandomOffset, 
	0.f
	);
	const FVector ProjectileLocation = TargetActor->GetActorLocation() + FVector(0.f, 0.f, 1000.f) + RandomOffset;
	const FRotator ProjectileDirection = ((TargetActor->GetActorLocation() - ProjectileLocation).GetSafeNormal()).Rotation();

	// 메테오 생성되는 위치의 생성되는 포탈 
	if (IsValid(PortalEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
		this, 
		PortalEffect,                  
		ProjectileLocation, 
		FRotator::ZeroRotator, 
		FVector::OneVector,          
		true                         
		);
	}

	// 메테오 생성
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileLocation, ProjectileDirection, SpawnParams);
	Projectile->LaunchProjectile((TargetActor->GetActorLocation() - ProjectileLocation).GetSafeNormal(), 1000.0f);
	Projectile->SetIsUse(true);

	// 메테오가 충돌하면 데미지를 처리할 GameplayEffectSpecHandle
	Projectile->DamageEffectSpecHandle = MakeMonsterBaseDamageEffectSpecHandle(DamageEffectClass, DamageScale);
}

void URPGGA_Monster_Attack_Meteor::ShowWeapon(FGameplayEventData PayloadData)
{
	// 파라곤 에셋의 문제로 무기를 보여주는 것을 AnimInstance로 처리함
	UCharacterAnimInstance* CharacterAnim = Cast<UCharacterAnimInstance>(CurrentActorInfo->GetAnimInstance());
	if (CharacterAnim)
	{
		CharacterAnim->SetbShowWeapon(true);
	}
}

void URPGGA_Monster_Attack_Meteor::HideWeapon(FGameplayEventData PayloadData)
{
	// 파라곤 에셋의 문제로 무기를 숨기는 것을 AnimInstance로 처리함
	UCharacterAnimInstance* CharacterAnim = Cast<UCharacterAnimInstance>(CurrentActorInfo->GetAnimInstance());
	if (CharacterAnim)
	{
		CharacterAnim->SetbShowWeapon(false);
	}
}
