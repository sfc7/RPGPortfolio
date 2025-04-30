// LJS


#include "GameAbilitySystem/GamePlayAbility/Player/PlayerGameplayAbility.h"
#include "Character/PlayerCharacter.h"
#include "Controller/PlayerCharacterController.h"
#include "Component/Player/PlayerCombatComponent.h"

APlayerCharacter* UPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!PlayerCharacter.IsValid())
	{
		PlayerCharacter = CastChecked<APlayerCharacter>(CurrentActorInfo->AvatarActor);
		return PlayerCharacter.Get();
	}
	
	return nullptr;
}

APlayerCharacterController* UPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!PlayerCharacterController.IsValid())
	{
		PlayerCharacterController = CastChecked<APlayerCharacterController>(CurrentActorInfo->PlayerController);
		return PlayerCharacterController.Get();
	}
	
	return nullptr;
}

UPlayerCombatComponent* UPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}
