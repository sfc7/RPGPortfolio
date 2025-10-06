// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/PlayerGameplayAbility.h"
#include "RPGGA_Player_RegenMp.generated.h"

/**
 * URPGGA_Player_RegenMp
 * 
 * 플레이어용 마나 리젠 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_RegenMp : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_RegenMp();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 마나 리젠 UGameplayEffect 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> RegenEffectClass;

	//@ 현재 활성화된 마나 리젠 UGameplayEffect 핸들
	UPROPERTY()
	FActiveGameplayEffectHandle CurrentRegenHandle;
};
