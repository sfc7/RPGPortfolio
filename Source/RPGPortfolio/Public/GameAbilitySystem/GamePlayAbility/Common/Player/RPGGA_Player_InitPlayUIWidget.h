// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_InitPlayUIWidget.generated.h"

class URPGWidgetBase;

UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_InitPlayUIWidget : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URPGWidgetBase> WidgetClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
