// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

UCLASS()
class RPGPORTFOLIO_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EProjectilePolicy ProjectilePolicy = EProjectilePolicy::Hit;
};
