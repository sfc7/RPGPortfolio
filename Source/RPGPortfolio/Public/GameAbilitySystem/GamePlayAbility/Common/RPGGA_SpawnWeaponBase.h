// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "RPGGA_SpawnWeaponBase.generated.h"

class AWeaponBase;
class ARPGCharacterBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_SpawnWeaponBase : public URPGGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_SpawnWeaponBase();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ARPGCharacterBase> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketNameToAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTagtoRegister;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldRegisterEquipWeapon = false;
};
