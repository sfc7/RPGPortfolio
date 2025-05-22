// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RPGStructTypes.h"
#include "CombatComponentBase.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	CarriedWeapon,
	LeftHand,
	RightHand
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UCombatComponentBase : public UActorComponent
{
	GENERATED_BODY()

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
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RegisterSpawnedWeapon(FGameplayTag _WeaponTagToRegister, AWeaponBase* _WeaponToRegister, bool bEquippedWeaponState = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCharacterEquippedWeaponByTag(FGameplayTag _WeaponTagToGet) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(AWeaponBase* _Weapon, bool _bShouldEnable);
	
	UFUNCTION(BlueprintCallable)
	void ToggleCurrentWeaponCollision(bool _bShouldEnable, EToggleDamageType _ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable)
	void ToggleCarriedWeaponCollision(AWeaponBase* _ToggleWeapon, bool _bShouldEnable, EToggleDamageType _ToggleDamageType = EToggleDamageType::CarriedWeapon);

	virtual void OnHitTargetActor(AActor* _HitActor, float _WeaponBaseDamage, EWeaponAttackType AttackType);
	virtual void OnWeaponPulledFromTargetActor(AActor* _InteractedActor, float _WeaponBaseDamage, EWeaponAttackType AttackType);


protected:
	TArray<AActor*> OVerlappedActors;
	
private:
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, AWeaponBase*> CharacterWeapons;
};
