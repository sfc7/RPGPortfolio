// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "RPGGA_Player_SpawnWeapon.generated.h"

class APlayerCharacter;
class APlayerWeapon;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_SpawnWeapon : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_SpawnWeapon();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APlayerWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketNameToAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTagtoRegister;
};
