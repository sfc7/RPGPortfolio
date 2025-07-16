// LJS

#pragma once

#include "CoreMinimal.h"
#include "WorldStatic/Projectile/PooledActor.h"
#include "GameplayEffectTypes.h"
#include "ProjectileBase.generated.h"

UENUM(BlueprintType)
enum class EProjectilePolicy : uint8
{
	Hit,
	BeinOverlap,
};

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
struct FGameplayEventData;
class UParticleSystemComponent;

UCLASS()
class RPGPORTFOLIO_API AProjectileBase : public APooledActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	UPROPERTY(BlueprintReadOnly)
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovementComponent() {return ProjectileMovementComponent;}

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(const FVector& LaunchDirection, float LaunchSpeed = 700.0f);

	virtual void SetIsUse(bool NewIsUse) override;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EProjectilePolicy ProjectilePolicy = EProjectilePolicy::Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ImpactNiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ImpactParticleEffect;



private:
	void HandleApplyProjectileDamage(APawn* HitPawn,const FGameplayEventData& Payload);
};
