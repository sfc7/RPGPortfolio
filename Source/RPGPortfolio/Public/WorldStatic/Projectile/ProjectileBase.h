// LJS

#pragma once

#include "CoreMinimal.h"
#include "WorldStatic/Projectile/PooledActor.h"
#include "GameplayEffectTypes.h"
#include "ProjectileBase.generated.h"

//@ 프로젝타일 정책 열거형
UENUM(BlueprintType)
enum class EProjectilePolicy : uint8
{
	//@ 히트 기반
	Hit,
	//@ 오버랩 기반
	BeinOverlap,
};

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
struct FGameplayEventData;
class UParticleSystemComponent;

/**
 * AProjectileBase
 * 
 * 스킬 등에 사용할 Projectile 클래스 Base
 */
UCLASS()
class RPGPORTFOLIO_API AProjectileBase : public APooledActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	//@ 데미지 이펙트 스펙 핸들 (게임플레이 이펙트 적용용)
	UPROPERTY(BlueprintReadOnly)
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovementComponent() {return ProjectileMovementComponent;}

	//@ 방향과 속도 설정해서 Projectile 발사 
	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(const FVector& LaunchDirection, float LaunchSpeed = 700.0f);

	//@ 오브젝트풀링용 사용 상태 설정
	virtual void SetIsUse(bool NewIsUse) override;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//@ 충돌 처리
	UFUNCTION()
	void ImpactProjectile(const FVector& HitLocation);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* CollisionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UParticleSystemComponent* CascadeComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	//@ Projectile 정책 (히트/오버랩)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EProjectilePolicy ProjectilePolicy = EProjectilePolicy::Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ImpactNiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ImpactParticleEffect;


private:
	//@ Projectile 데미지 적용 처리
	void HandleApplyProjectileDamage(APawn* HitPawn,const FGameplayEventData& Payload);
};
