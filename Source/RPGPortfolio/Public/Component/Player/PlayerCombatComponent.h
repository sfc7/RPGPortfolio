// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/CombatComponentBase.h"
#include "GameplayTagContainer.h"
#include "PlayerCombatComponent.generated.h"

class APlayerWeapon;

/**
 * UPlayerCombatComponent
 * 
 * PlayerCharacter의 전투에 사용할 컴포넌트
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCombatComponent : public UCombatComponentBase
{
	GENERATED_BODY()
public:
	//@ 태그로 플레이어 무기 조회
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	APlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const;

	//@ 현재 장착된 플레이어 무기 반환
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	APlayerWeapon* GetPlayerCurrentEquippedWeapon() const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel() const;

	//@ 충돌 토글 시작시점
	virtual void OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName);
	
	//@ 충돌 토글 종료시점
	virtual void OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType);

protected:
	virtual void BeginPlay() override;

	//@ 패링 공격용 IMC 변경함수
	void OnParryingStateChange(const FGameplayTag CallbackTag, int32 NewCount);

	//@ 패링관련 GameplayTag초기화
	void InitRegisterGameplayTagEvent();

	//@ 패링 공격용 IMC 우선순위
	int32 MappingPriority = 1; 
};
