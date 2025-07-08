// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class RPGPORTFOLIO_API APooledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APooledActor();

	UFUNCTION(BlueprintCallable)
	bool GetIsUse() const { return IsUse; }

	UFUNCTION(BlueprintCallable)
	float GetTimeToLive() const { return TimeToLive; }

	UFUNCTION(BlueprintCallable)
	virtual void SetIsUse(bool NewIsUse);

	UFUNCTION(BlueprintCallable)
	void SetTimeToLive(float NewTimeToLive) { TimeToLive = NewTimeToLive; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsUse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToLive;

	FTimerHandle LiveTimerHandle;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
