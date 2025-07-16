// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_TeleportDropKick.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerAttributeSet.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Player/PlayerSkillComponent.h"

URPGGA_Player_TeleportDropKick::URPGGA_Player_TeleportDropKick()
{
}

void URPGGA_Player_TeleportDropKick::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayerCharacterBase* OwnerCharacter = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!OwnerCharacter)
	{
		return;
	}
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent())
	{
		int32 QuickSlotIndex = GetPlayerCharacterFromActorInfo()->GetPlayerSkillComponent()->FindQuickSlotIndexByTag(RPGGameplayTag::Player_Ability_Skill_DropKick);
        
		GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast(
			QuickSlotIndex, 
			RPGGameplayTag::Player_Ability_Skill_DropKick, 
			GetCooldownTimeRemaining(), 
			GetCooldownTimeRemaining()
		);
	}
	
	bool bRotate = FindNearestEnemyBeforeAttack(1000.f);
	
	if (RotateTargetActor)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RotateTargetActor);
		if (TargetASC && !TargetASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
		{
			RotateDirection();
		}
		else
		{
			ForwardDirection();
		}
	}
	else
	{
		ForwardDirection();
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_TeleportDropKick"), AttackMontage, 1.0f, NAME_None,
		true, 1.0f, false);
    
	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_TeleportDropKick::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_AttackHit_Melee, nullptr, false, true);

	GameplayEventTask->EventReceived.AddDynamic(this, &URPGGA_Player_TeleportDropKick::ApplyEffectsSpecHandleToTargetCallback);
	GameplayEventTask->ReadyForActivation();
    
	CommitAbility(Handle, ActorInfo, ActivationInfo);
}

void URPGGA_Player_TeleportDropKick::ForwardDirection()
{
	if (WarpTargetNameDistance == FName(TEXT("None")) || WarpTargetNameDistance == FName(TEXT("")))
	{
		return;
	}
    
	FVector MyLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector MyForward = GetPlayerCharacterFromActorInfo()->GetActorForwardVector();
	FVector WarpLocation = MyLocation + (MyForward * 500.0f);
    
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(WarpTargetNameDistance, WarpLocation);
}

void URPGGA_Player_TeleportDropKick::RotateDirection()
{
	if (RotateTargetActor)
	{
		FVector TargetLocation = RotateTargetActor->GetActorLocation();
		FVector MyLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
		FVector DirectionToTarget = (TargetLocation - MyLocation).GetSafeNormal();
        
		FVector WarpLocation = TargetLocation - (DirectionToTarget * 50.0f);
		FRotator WarpRotation = UKismetMathLibrary::FindLookAtRotation(WarpLocation, TargetLocation);

		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		   WarpTargetNameDirection, FVector::ZeroVector, FRotationMatrix::MakeFromX(DirectionToTarget).Rotator());
		
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(WarpTargetNameDistance, WarpLocation);
	}	
}

void URPGGA_Player_TeleportDropKick::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_TeleportDropKick::ApplyEffectsSpecHandleToTargetCallback(FGameplayEventData PayloadData)
{
	AActor* LocalTargetActor = const_cast<AActor*>(PayloadData.Target.Get());
	
	float WeaponDamage = GetPlayerCharacterFromActorInfo()->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()->WeaponDefaultData.WeaponBaseDamage;
	FGameplayEffectSpecHandle SpecHandle = MakePlayerSkillDamageEffectSpecHandle(DamageEffectClass, WeaponDamage, DamageScale);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		ASC->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Heavy, EffectContext);
	}
	
	FActiveGameplayEffectHandle HitReactGameplayEffectHandle = ApplyEffectsSpecHandleToTarget(LocalTargetActor, SpecHandle);
	if (HitReactGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(LocalTargetActor, RPGGameplayTag::Character_Event_HitReact, PayloadData);
	}
	
	FVector AttackDirection = (LocalTargetActor->GetActorLocation() - GetPlayerCharacterFromActorInfo()->GetActorLocation()).GetSafeNormal();
	FVector HitLocation = PayloadData.ContextHandle.GetHitResult()->Location;
	FGameplayCueParameters AttackHitGCParam;
	AttackHitGCParam.Normal = AttackDirection;
	AttackHitGCParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	AttackHitGCParam.Location = HitLocation;	
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->ExecuteGameplayCue(RPGGameplayTag::GameplayCue_Player_Fighter_Effect_AttackHit_Melee_Heavy, AttackHitGCParam);
}
