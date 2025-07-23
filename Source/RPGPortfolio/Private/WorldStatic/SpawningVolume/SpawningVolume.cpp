// LJS


#include "WorldStatic/SpawningVolume/SpawningVolume.h"

#include "Components/BoxComponent.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "Kismet/KismetMathLibrary.h"

ASpawningVolume::ASpawningVolume()
{
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	RootComponent = SpawningBox;
}

void ASpawningVolume::BeginPlay()
{
	Super::BeginPlay();

	UDungeonProgressManager* DungeonProgressManager = GetWorld()->GetGameInstance()->GetSubsystem<UDungeonProgressManager>();
	if (!DungeonProgressManager) return;

	DungeonProgressManager->RegisterSpawningVolume(this);
}


FVector ASpawningVolume::GetRandomSpawnLocation()
{
	FVector VolumeOrigin = SpawningBox->Bounds.Origin;
	FVector VolumeExtent = SpawningBox->Bounds.BoxExtent;
	
    return UKismetMathLibrary::RandomPointInBoundingBox(VolumeOrigin, VolumeExtent);
}


AActor* ASpawningVolume::SpawnActorAtLocalLocation(TSubclassOf<AActor> ActorClass, const FVector& LocalLocation)
{
	if (!ActorClass) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ActorClass, LocalLocation, FRotator::ZeroRotator, SpawnParams);
	return Spawned;
}

AActor* ASpawningVolume::RandomSpawnActorInBox(TSubclassOf<AActor> ActorClass)
{
	if (!ActorClass) return nullptr;

	FVector WorldRandom = GetRandomSpawnLocation();
	return SpawnActorAtLocalLocation(ActorClass, WorldRandom);
}

AActor* ASpawningVolume::SpawnActorAtBoxLocalLocation(TSubclassOf<AActor> ActorClass, const FVector& BoxLocalLocation)
{
	if (!ActorClass) return nullptr;

	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	if (FMath::Abs(BoxLocalLocation.X) > BoxExtent.X ||
		FMath::Abs(BoxLocalLocation.Y) > BoxExtent.Y ||
		FMath::Abs(BoxLocalLocation.Z) > BoxExtent.Z)
	{
		return nullptr;
	}

	FVector WorldLocation = SpawningBox->Bounds.Origin + BoxLocalLocation;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ActorClass, WorldLocation, FRotator::ZeroRotator, SpawnParams);

	return Spawned;
}