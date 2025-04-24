// LJS


#include "GameAbilitySystem/GamePlayAbility/Player/State/Player_EnterCombat.h"

UPlayer_EnterCombat::UPlayer_EnterCombat()
{
	AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnGiven;
}

void UPlayer_EnterCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}
