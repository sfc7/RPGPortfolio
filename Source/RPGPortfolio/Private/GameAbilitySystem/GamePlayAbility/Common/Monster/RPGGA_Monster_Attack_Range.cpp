// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_Range.h"
#include "Character/MonsterCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "WorldStatic/Projectile/ProjectileBase.h"

URPGGA_Monster_Attack_Range::URPGGA_Monster_Attack_Range()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_Range::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	// 발사체 패턴 애니메이션 재생 AbilityTask 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Attack_Range"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_Range::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	// 애니메이션으로 부터 GameplayEvent를 기다림, 여기에는 원거리 발사체 생성
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
		);

	WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
	WaitGameplayEvent->ReadyForActivation();	
}

void URPGGA_Monster_Attack_Range::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Monster_Attack_Range::SpawnProjectile(FGameplayEventData PayloadData)
{
	// 발사체가 생성되는 랜덤 위치
	const FVector ProjectileLocation = GetOwningComponentFromActorInfo()->GetSocketLocation(FName(TEXT("ProjectileSocket")));
	const FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetMonsterCharacterFromActorInfo();
	SpawnParams.Instigator = GetMonsterCharacterFromActorInfo();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 발사체 생성
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileLocation, ProjectileDirection, SpawnParams);
	Projectile->LaunchProjectile(GetMonsterCharacterFromActorInfo()->GetActorForwardVector(), 700.0f);
	Projectile->SetIsUse(true);
	// 발사체가 충돌하면 데미지를 처리할 GameplayEffectSpecHandle
	Projectile->DamageEffectSpecHandle = MakeMonsterBaseDamageEffectSpecHandle(DamageEffectClass, DamageScale);
}
