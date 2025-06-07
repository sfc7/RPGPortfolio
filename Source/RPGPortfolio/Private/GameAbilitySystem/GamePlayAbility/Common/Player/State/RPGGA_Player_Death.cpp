// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_Death.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Controller/PlayerCharacterController.h"

URPGGA_Player_Death::URPGGA_Player_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void URPGGA_Player_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_Death"), DeathMontage, 1.0f,  NAME_None,
	true, 1.0f, 0.f,false);

	
	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Death::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Death::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_Death::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_Death::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();
}

void URPGGA_Player_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetOwningComponentFromActorInfo()->bPauseAnims = true;
	GetPlayerCharacterFromActorInfo()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	APlayerCharacterController* PC = GetPlayerControllerFromActorInfo();
	if (PC)
	{
		FInputModeUIOnly InputModeUI;
		PC->SetInputMode(InputModeUI);
		PC->SetShowMouseCursor(true);
	}
}

void URPGGA_Player_Death::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
