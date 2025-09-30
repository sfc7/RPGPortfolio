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

/**
 * UObjectPoolComponent
 * 
 * 오브젝트 풀링 시스템 관리 컴포넌트, 오브젝트를 미리 생성하여 성능 최적화, 태그 기반 풀 관리, 오브젝트 검색
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPORTFOLIO_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolComponent();

protected:
	virtual void BeginPlay() override;

	//@ GameplayTag별 Projectile 다중 풀
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Multi Pool")
	TMap<FGameplayTag, FProjectilePoolData> ProjectilePools;

public:
	UFUNCTION(BlueprintCallable)
	APooledActor* SpawnFromPool(FGameplayTag PoolTag, FVector ObjectLocation, FRotator ObjectRotation);

private:
	void CreatePool(FGameplayTag PoolTag, FProjectilePoolData& PoolData);
	//@ 사용 가능한 오브젝트 검색
	APooledActor* FindAvailableActor(FGameplayTag PoolTag);
};