// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_NenShotSkill.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "RPGStructTypes.h"
#include "WorldStatic/Weapon/WeaponBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Component/Player/PlayerCombatComponent.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Player/PlayerUIComponent.h"

URPGGA_Player_NenShotSkill::URPGGA_Player_NenShotSkill()
{
}

void URPGGA_Player_NenShotSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnSkillCooldownBeginDelegate.Broadcast()
	
	UPlayerCombatComponent* CombatComponent = GetPlayerCombatComponentFromActorInfo();
	if (CombatComponent)
	{
		AWeaponBase* CurrentWeapon = CombatComponent->GetCharacterCurrentEquippedWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->SetCurrentAttackType(EWeaponAttackType::Light);
		}
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,TEXT("Player_NenShot_Skill"), NenShotMontage, 1.0f,  NAME_None,
		true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_NenShotSkill::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_NenShotSkill::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_NenShotSkill::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_NenShotSkill::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();
}

void URPGGA_Player_NenShotSkill::Attack()
{
}

void URPGGA_Player_NenShotSkill::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
