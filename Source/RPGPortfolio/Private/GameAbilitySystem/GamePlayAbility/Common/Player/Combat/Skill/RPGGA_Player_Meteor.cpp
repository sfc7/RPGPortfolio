// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/RPGGA_Player_Meteor.h"

URPGGA_Player_Meteor::URPGGA_Player_Meteor()
{
}

void URPGGA_Player_Meteor::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGGA_Player_Meteor::OnEndAbilityCallback()
{
}

void URPGGA_Player_Meteor::SpawnProjectile(FGameplayEventData PayloadData)
{
}
