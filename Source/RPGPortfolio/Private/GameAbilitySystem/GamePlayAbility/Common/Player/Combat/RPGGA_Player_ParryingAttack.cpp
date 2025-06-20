// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_ParryingAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "RPGStructTypes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"

URPGGA_Player_ParryingAttack::URPGGA_Player_ParryingAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_ParryingAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bool bRotate = FindNearestEnemyBeforeAttack(600.f); 

	if (bRotate)
	{		
		URPGAT_Player_RotateTarget* RotateTickTask = URPGAT_Player_RotateTarget::ExecuteTaskOnTick(this);
		RotateTickTask->OnRotateTargetTaskTick.AddDynamic(this, &UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack);
		RotateTickTask->SetTargetRotation(FindRototation);
		RotateTickTask->OnRotationCompleted.AddDynamic(this, &URPGGA_Player_ParryingAttack::Attack);
		RotateTickTask->ReadyForActivation();
	}
	else
	{	
		Attack();
	}
}

void URPGGA_Player_ParryingAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGGA_Player_ParryingAttack::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_ParryingAttack::Attack()
{	
	UPlayerCombatComponent* CombatComponent = GetPlayerCombatComponentFromActorInfo();
	if (CombatComponent)
	{
		AWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCurrentEquippedWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->SetCurrentAttackType(EWeaponAttackType::Parrying);
		}
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,TEXT("Player_Parrying_Attack"), ParryingAttackMontage, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_ParryingAttack::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* AttackHitGE = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this,RPGGameplayTag::Character_Event_AttackHit_Melee,nullptr, false, true);

	AttackHitGE->EventReceived.AddDynamic(this, &ThisClass::ApplyEffectsSpecHandleToTargetCallback);
	AttackHitGE->ReadyForActivation();
}

void URPGGA_Player_ParryingAttack::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());
	float WeaponDamage = PayloadData.EventMagnitude;
	FGameplayEffectSpecHandle SpecHandle = MakePlayerBaseDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, RPGGameplayTag::Data_DamageType_SetByCaller_Parrying);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		ASC->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Sound_AttackHit_Parrying, EffectContext);
	}
	
	FActiveGameplayEffectHandle HitReactGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (HitReactGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}
	BP_ApplyGameplayEffectToOwner(GainMpEffectClass,GetAbilityLevel());
	
}
