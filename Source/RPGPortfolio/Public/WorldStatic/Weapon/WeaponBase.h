// LJS

#pragma once

#include "CoreMinimal.h"
#include "RPGStructTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class UBoxComponent;

DECLARE_DELEGATE_SixParams(FOnWeaponHitBegin, AActor*, float, float, EWeaponAttackType, EWeaponType, FName);
DECLARE_DELEGATE_FiveParams(FOnWeaponHitEnd, AActor*, float, float, EWeaponAttackType, EWeaponType);

/**
 * AWeaponBase
 * 
 * 모든 무기 타입 Base 클래스
 */
UCLASS()
class RPGPORTFOLIO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWeaponDefaultData WeaponDefaultData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	EWeaponType WeaponType;

	FOnWeaponHitBegin OnWeaponHitBegin;
	FOnWeaponHitEnd OnWeaponHitEnd;

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
