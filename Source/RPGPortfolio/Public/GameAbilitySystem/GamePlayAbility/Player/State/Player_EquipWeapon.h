// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "Player_EquipWeapon.generated.h"

class APlayerCharacter;
class APlayerWeapon;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayer_EquipWeapon : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayer_EquipWeapon();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APlayerWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketNameToAttach;
	
};
