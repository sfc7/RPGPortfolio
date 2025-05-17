// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_EnterCombat.h"

URPGGA_Player_EnterCombat::URPGGA_Player_EnterCombat()
{
	AbilityActivationPolicy = ERPGGameplayAbilityActivationType::OnGiven;
}

void URPGGA_Player_EnterCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}
