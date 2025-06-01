// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Montser_HitReact_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Montser_HitReact_Base : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> HitReactMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasHitReactMontagesToPlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> IsBeingAttackedGameplayEffect;
	
	UFUNCTION()
	void OnEndAbilityCallback();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseFaceAttacker = true;
};
