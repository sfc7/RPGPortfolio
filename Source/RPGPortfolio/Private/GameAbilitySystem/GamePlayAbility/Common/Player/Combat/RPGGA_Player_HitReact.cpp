// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_HitReact.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "RPGFunc.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/RPGGA_Player_Dodge.h"

URPGGA_Player_HitReact::URPGGA_Player_HitReact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OUT float HitReactDirection;
	AActor* Attacker = const_cast<AActor*>(TriggerEventData->Instigator.Get());
	check(Attacker);
	AActor* HitReactPlayer = Cast<AActor>(GetPlayerCharacterFromActorInfo());
	check(HitReactPlayer);
	
	UAnimMontage* MontageToPlay = nullptr;
	FGameplayTag HitReactGamePlayTag = URPGFunc::GetHitReactDirection(Attacker, HitReactPlayer, HitReactDirection);
	if (HitReactGamePlayTag == RPGGameplayTag::Character_Status_HitReact_Front)
	{
		MontageToPlay = HitReactMontage_Front;
		UE_LOG(LogTemp, Warning, TEXT("Front"));
	}
	else if (HitReactGamePlayTag == RPGGameplayTag::Character_Status_HitReact_Back)
	{
		MontageToPlay = HitReactMontage_Back;
		UE_LOG(LogTemp, Warning, TEXT("Back"));
	}
	else if (HitReactGamePlayTag == RPGGameplayTag::Character_Status_HitReact_Left)
	{
		MontageToPlay = HitReactMontage_Left;
		UE_LOG(LogTemp, Warning, TEXT("Left"));
	}
	else if (HitReactGamePlayTag == RPGGameplayTag::Character_Status_HitReact_Right)
	{
		MontageToPlay = HitReactMontage_Right;
		
	}

	if (IsValid(MontageToPlay))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
TEXT("Player_HitReact"), MontageToPlay, 1.0f, NAME_None,
true, 1.0f, false);
	
		PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_HitReact::OnEndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_HitReact::OnEndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Player_HitReact::OnEndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Player_HitReact::OnEndAbilityCallback);
		PlayMontageTask->ReadyForActivation();
	}
}

void URPGGA_Player_HitReact::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
