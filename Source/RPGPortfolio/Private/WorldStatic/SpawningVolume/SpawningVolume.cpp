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
	// 박스 영역의 중심점과 크기 가져오기
	const FVector VolumeOrigin = SpawningBox->Bounds.Origin;
	const FVector VolumeExtent = SpawningBox->Bounds.BoxExtent;

	// 박스 영역 내 랜덤 위치 생성
    return UKismetMathLibrary::RandomPointInBoundingBox(VolumeOrigin, VolumeExtent);
}


AActor* ASpawningVolume::SpawnActorAtWorldLocation(TSubclassOf<AActor> ActorClass, const FVector& LocalLocation)
{
	if (!IsValid(ActorClass)) return nullptr;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 월드 좌표에 액터 스폰
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ActorClass, LocalLocation, FRotator::ZeroRotator, SpawnParams);
	return Spawned;
}

AActor* ASpawningVolume::RandomSpawnActorInBox(TSubclassOf<AActor> ActorClass)
{
	if (!IsValid(ActorClass)) return nullptr;

	// 랜덤 위치 생성 및 스폰
	FVector WorldRandom = GetRandomSpawnLocation();
	return SpawnActorAtWorldLocation(ActorClass, WorldRandom);
}

AActor* ASpawningVolume::SpawnActorAtBoxLocalLocation(TSubclassOf<AActor> ActorClass, const FVector& BoxLocalLocation)
{
	if (!ActorClass) return nullptr;

	// 박스 경계 검증
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	if (FMath::Abs(BoxLocalLocation.X) > BoxExtent.X ||
		FMath::Abs(BoxLocalLocation.Y) > BoxExtent.Y ||
		FMath::Abs(BoxLocalLocation.Z) > BoxExtent.Z)
	{
		return nullptr;
	}

	// 로컬 좌표를 월드 좌표로 변환, 씬 컴포넌트 기준으로 로컬 좌표 더해서 만들기
	const FVector WorldLocation = SpawningBox->Bounds.Origin + BoxLocalLocation;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 월드 좌표에 액터 스폰
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ActorClass, WorldLocation, FRotator::ZeroRotator, SpawnParams);

	return Spawned;
}