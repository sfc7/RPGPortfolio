// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_InitPlayUIWidget.h"
#include "Widget/RPGWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Controller/PlayerCharacterController.h"

void URPGGA_Player_InitPlayUIWidget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	URPGWidgetBase* PlayerUI = CreateWidget<URPGWidgetBase>(GetPlayerControllerFromActorInfo(), WidgetClass);
	PlayerUI->AddToViewport();
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Player_InitPlayUIWidget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
