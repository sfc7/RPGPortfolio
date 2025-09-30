// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/CombatComponentBase.h"
#include "MonsterCombatComponent.generated.h"

/**
 * UMonsterCombatComponent
 * 
 * 몬스터의 전투에 사용할 컴포넌트
 */
UCLASS()
class RPGPORTFOLIO_API UMonsterCombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()
public:
	//@ 생성된 무기를 몬스터에 등록
	virtual void RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState = false) override;

	//@ 충돌 토글 시작시점
	virtual void OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName = NAME_None) override;

	//@ 충돌 토글 종료시점
	virtual void OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType) override;

protected:
	//@ 무기를 사용하지 않고 부위를 이용한 공격 토글시
	virtual void ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType) override;
};
