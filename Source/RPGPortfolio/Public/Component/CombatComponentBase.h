// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CombatComponentBase.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponCollision(bool _bShouldEnable, EToggleDamageType _ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);
private:
	TMap<FGameplayTag, AWeaponBase*> CharacterWeapons;
};
