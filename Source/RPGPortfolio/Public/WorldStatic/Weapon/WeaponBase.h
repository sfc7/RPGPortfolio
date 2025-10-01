// LJS

#pragma once

#include "CoreMinimal.h"
#include "RPGStructTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class UBoxComponent;

//@ 히트 시작 델리게이트
DECLARE_DELEGATE_SixParams(FOnWeaponHitBegin, AActor*, float, float, EWeaponAttackType, EWeaponType, FName);
//@ 히트 종료 델리게이트 
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
	// Sets default values for this actor's properties
	AWeaponBase();

	//@ 무기 기본 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWeaponDefaultData WeaponDefaultData;

	//@ 무기 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	EWeaponType WeaponType;

	//@ 히트 시작 델리게이트
	FOnWeaponHitBegin OnWeaponHitBegin;
	//@ 히트 종료 델리게이트 
	FOnWeaponHitEnd OnWeaponHitEnd;

	//@ 현재 공격의 타입 설정
	void SetCurrentAttackType(EWeaponAttackType AttackType);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UBoxComponent* WeaponCollisionBox;

	//@ 현재 공격 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponAttackType CurrentAttackType;
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox;}

};
