// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "RPGGA_SpawnWeaponBase.generated.h"

class AWeaponBase;
class ARPGCharacterBase;
/**
 * URPGGA_SpawnWeaponBase
 * 
 * 무기 스폰 게임플레이 어빌리티
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

	//@ 소유자 캐릭터 참조
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ARPGCharacterBase> OwnerCharacter;

	//@ 스폰할 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponBase> WeaponClass;

	//@ 무기 부착 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketNameToAttach;

	//@ 무기 등록용 게임플레이 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTagtoRegister;

	//@ 무기 장착 등록 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldRegisterEquipWeapon = false;
};
