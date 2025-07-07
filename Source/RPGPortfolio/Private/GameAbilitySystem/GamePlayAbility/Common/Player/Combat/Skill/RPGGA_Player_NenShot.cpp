// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_NenShot.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "WorldStatic/Projectile/ProjectileBase.h"

URPGGA_Player_NenShot::URPGGA_Player_NenShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_NenShot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AttackMontage)
	{

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Attack_Range"), AttackMontage, 1.0f,  NAME_None,
	true, 1.0f, false);

		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_NenShot::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_SpawnProjectile, nullptr, false, true
		);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
		WaitGameplayEvent->ReadyForActivation();
	}
}

void URPGGA_Player_NenShot::OnEndAbilityCallback()
{
}

void URPGGA_Player_NenShot::SpawnProjectile(FGameplayEventData PayloadData)
{
	FVector ProjectileLocation = GetOwningComponentFromActorInfo()->GetSocketLocation(FName(TEXT("ProjectileSocket")));
	FRotator ProjectileDirection = UKismetMathLibrary::MakeRotFromX(GetPlayerCharacterFromActorInfo()->GetActorForwardVector());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetPlayerCharacterFromActorInfo();
	SpawnParams.Instigator = GetPlayerCharacterFromActorInfo();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
}
