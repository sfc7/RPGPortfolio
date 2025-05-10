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
	}
	
	return PlayerCharacter.IsValid() ? PlayerCharacter.Get() : nullptr;
}

APlayerCharacterController* UPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!PlayerCharacterController.IsValid())
	{
		PlayerCharacterController = CastChecked<APlayerCharacterController>(CurrentActorInfo->PlayerController);
	}
	
	return PlayerCharacterController.IsValid() ? PlayerCharacterController.Get() : nullptr;
}

UPlayerCombatComponent* UPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}
