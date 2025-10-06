// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_RollShoot.generated.h"

class AProjectileBase;
/**
 * URPGGA_Monster_Attack_RollShoot
 * 
 * 보스 몬스터 구르고 총을 3번 발사하는 패턴 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_RollShoot : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_RollShoot();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 총 발사체 생성
	UFUNCTION()
	void SpawnBullet(FGameplayEventData PayloadData);

	//@ 데미지 계산용 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//@ 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;

	//@ 공격 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 총의 타겟 탐지에 사용할 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletTraceRadius;

	//@ 총 발사 시 Muzzle 효과를 적용할 소켓 FName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName BulletMuzzle;

	//@ 발사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;

	//@ 사격 시 필요한 해당 무기를 보여줌
	UFUNCTION()
	void ShowWeapon(FGameplayEventData PayloadData);

	//@ 사격 시 무기를 다시 숨김 
	UFUNCTION()
	void HideWeapon(FGameplayEventData PayloadData);
};
