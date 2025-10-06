// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_NenShot.generated.h"

class AProjectileBase;
/**
 * URPGGA_Player_NenShot
 * 
 * 플레이어 넨샷 스킬 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_NenShot : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_NenShot();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 스킬 실행 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 스킬 실행 관련 로직
	UFUNCTION()
	void Attack();

	//@ 발사체 생성
	UFUNCTION()
	void SpawnProjectile(FGameplayEventData PayloadData);

	//@ 발사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;


};
