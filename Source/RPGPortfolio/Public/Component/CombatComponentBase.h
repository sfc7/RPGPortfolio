// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RPGStructTypes.h"
#include "CombatComponentBase.generated.h"

class AWeaponBase;
class URPGAbilitySystemComponent;

//@ 충돌에 사용할 데미지 토글 부위
UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	CarriedWeapon,
	LeftHand,
	RightHand
};

/**
 * UCombatComponentBase
 * 
 * 전투에 사용할 컴포넌트의 Base
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UCombatComponentBase : public UActorComponent
{
	GENERATED_BODY()

	//@ 컴포넌트 오너용 템플릿 함수
#pragma region GetOwningPawn(), GetOwningController()
protected:
	template<class T>
	T* GetOwningPawn() const
	{
		return CastChecked<T>(GetOwner());
	}
	
	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		return GetOwningPawn<APawn>()->GetController<T>();
	}
#pragma endregion
	
public:
	//@ 생성된 무기를 캐릭터에 등록
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState = false);
	
	//@ Gameplay태그로 소지 중인 무기 조회
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const;

	//@ 현재 장착된 무기 반환
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	//@ 태그로 장착된 무기 조회 (장착 상태 확인)
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCharacterEquippedWeaponByTag(FGameplayTag _WeaponTagToGet) const;

	//@ 현재 장착된 무기의 태그
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag CurrentEquippedWeaponTag;

	//@ 무기의 충돌 상태 설정
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(AWeaponBase* _Weapon, bool _bShouldEnable);

	//@ 현재 무기의 충돌 상태 설정
	UFUNCTION(BlueprintCallable)
	void ToggleCurrentWeaponCollision(bool _bShouldEnable, EToggleDamageType _ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	//@ 소지한 무기용 충돌 상태 설정
	UFUNCTION(BlueprintCallable)
	void ToggleCarriedWeaponCollision(AWeaponBase* _ToggleWeapon, bool _bShouldEnable, EToggleDamageType _ToggleDamageType = EToggleDamageType::CarriedWeapon);

	//@ 충돌 토글 시작시점
	virtual void OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName);

	//@ 충돌 토글 종료시점
	virtual void OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType);


protected:
	//@ 무기를 사용하지 않고 부위를 이용한 공격 토글시
	virtual void ToggleBodyCollisionBoxCollsion(bool _bShouldEnable, EToggleDamageType _ToggleDamageType);

	//@ 현재 겹치고 있는 액터들, 중복 히트 방지용
	TArray<AActor*> OverlappedActors;

	UFUNCTION(BlueprintPure, Category = "GA")
	URPGAbilitySystemComponent* GetRPGAbilitySystemComponent() const;
	
private:
	// 캐릭터가 소지한 무기들, 태그로 관리
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, AWeaponBase*> CharacterWeapons;
};