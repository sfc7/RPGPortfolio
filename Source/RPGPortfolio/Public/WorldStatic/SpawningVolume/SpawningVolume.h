// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawningVolume.generated.h"

class UBoxComponent;
class ATargetPoint;

UCLASS()
class RPGPORTFOLIO_API ASpawningVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawningVolume();

	UBoxComponent* GetSpawningBox() const { return SpawningBox; }

	FString GetVolumeName() const { return VolumeName; }

	UFUNCTION(BlueprintPure)
	FVector GetRandomSpawnLocation();
	
	UFUNCTION(BlueprintCallable)
	AActor* SpawnActorAtLocalLocation(TSubclassOf<AActor> ActorClass, const FVector& Location);

	UFUNCTION(BlueprintCallable)
	AActor* RandomSpawnActorInBox(TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnActorAtBoxLocalLocation(TSubclassOf<AActor> ActorClass, const FVector& BoxLocalLocation);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SpawningBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString VolumeName;
};
