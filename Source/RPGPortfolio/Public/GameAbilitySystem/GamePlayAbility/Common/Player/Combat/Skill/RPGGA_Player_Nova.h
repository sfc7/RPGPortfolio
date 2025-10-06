// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/Skill/PlayerAttackSkillGameplayAbility.h"
#include "RPGGA_Player_Nova.generated.h"

class AProjectileBase;
/**
 * URPGGA_Player_Nova
 * 
 * 플레이어 노바 스킬 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Player_Nova : public UPlayerAttackSkillGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Player_Nova();

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

	//@ 발사체 생성 개수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ProjectileCount = 20;

	//@ 노바 스킬 발사할 각도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StartRadius = 100.0f;

	//@ 발사체 생성 높이
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ProjectileHeight = 30.0f; 
};
