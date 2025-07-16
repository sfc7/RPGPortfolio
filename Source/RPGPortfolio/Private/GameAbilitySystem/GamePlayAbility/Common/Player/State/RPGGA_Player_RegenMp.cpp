// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_RegenMp.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"

URPGGA_Player_RegenMp::URPGGA_Player_RegenMp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Player_RegenMp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	URPGAbilitySystemComponent* ASC = GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent();
    
	if (CurrentRegenHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(CurrentRegenHandle);
		CurrentRegenHandle.Invalidate();
	}

	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(RegenEffectClass, 1, EffectContextHandle);
	if (SpecHandle.IsValid())
	{
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
		CurrentRegenHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void URPGGA_Player_RegenMp::OnEndAbilityCallback()
{
	
}
