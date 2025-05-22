// LJS

#pragma once

#include "CoreMinimal.h"
#include "RPGStructTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class UBoxComponent;

DECLARE_DELEGATE_ThreeParams(FOnWeaponHitTarget, AActor*, float, EWeaponAttackType);


UCLASS()
class RPGPORTFOLIO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWeaponDefaultData WeaponDefaultData;
	
	FOnWeaponHitTarget OnWeaponHitTarget;
	FOnWeaponHitTarget OnWeaponPulledFromTarget;

	void SetCurrentAttackType(EWeaponAttackType AttackType);
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UBoxComponent* WeaponCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponAttackType CurrentAttackType;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox;}

};
