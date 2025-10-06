// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_Attack_Range.generated.h"

class AProjectileBase;
/**
 * URPGGA_Monster_Attack_Range
 * 
 * 몬스터 기본 원거리 공격 패턴 GameplayAbility
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_Attack_Range : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_Attack_Range();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//@ 공격 Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	//@ 어빌리티 종료 콜백 함수
	UFUNCTION()
	void OnEndAbilityCallback();

	//@ 원거리 공격 발사체 생성
	UFUNCTION()
	void SpawnProjectile(FGameplayEventData PayloadData);

	//@ 발사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> ProjectileClass;

	//@ 데미지 계산용 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//@ 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DamageScale;
};
