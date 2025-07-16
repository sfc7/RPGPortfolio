// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/GameplayAbility.h"
#include "ObjectPoolComponent.generated.h"

class APooledActor;

class APooledActor;

USTRUCT(BlueprintType)
struct FProjectilePoolData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APooledActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PoolSize = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TimeToLive = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APooledActor*> ObjectPool;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPORTFOLIO_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Multi Pool")
	TMap<FGameplayTag, FProjectilePoolData> ProjectilePools;

public:
	UFUNCTION(BlueprintCallable)
	APooledActor* SpawnFromPool(FGameplayTag PoolTag, FVector ObjectLocation, FRotator ObjectRotation);

private:
	void CreatePool(FGameplayTag PoolTag, FProjectilePoolData& PoolData);
	APooledActor* FindAvailableActor(FGameplayTag PoolTag);
};