// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

class APooledActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolComponent();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<APooledActor> GetPooledActorClass() const { return PooledActorClass; }

	UFUNCTION(BlueprintCallable)
	void SetPooledActor(TSubclassOf<APooledActor> NewPooledActorClass) { PooledActorClass = NewPooledActorClass; }

	UFUNCTION(BlueprintCallable)
	void InitializePool();

	UFUNCTION(BlueprintCallable)
	APooledActor* FindAvailableActor();

	UFUNCTION(BlueprintCallable)
	APooledActor* SpawnFromPool(FVector ObjectLocation, FRotator ObjectRotation);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APooledActor> PooledActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PoolSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TimeToLive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APooledActor*> ObjectPool;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
