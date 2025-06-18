// LJS


#include "GameMode/DungeonGameMode.h"

#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "GameMode/RPGGameManager.h"

void ADungeonGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetDungeonState(EDungeonState::NormalMonsterPhase);

	AsyncLoadSoftClassptrMonster();

}

void ADungeonGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DungeonState == EDungeonState::NormalMonsterPhase || DungeonState == EDungeonState::BossMonsterPhase)
	{
		GameTime += DeltaTime;

		if (GameTime >= 5.f && !flagspawn)
		{
			SpawnMonster();
			flagspawn = true;
		}
	}

}

void ADungeonGameMode::SetDungeonState(EDungeonState GamemodeState)
{
	DungeonState = GamemodeState;
}

void ADungeonGameMode::AsyncLoadSoftClassptrMonster()
{
	TArray<FName> RowNames = MonsterDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FMonsterInDungeonTable* RowMonsterData = MonsterDataTable->FindRow<FMonsterInDungeonTable>(RowName, TEXT(""));
		if (RowMonsterData)
		{
			const FMonsterInfo& RowMonsterInfo = RowMonsterData->MonsterInfo;
			
			if (RowMonsterInfo.MonsterSoftClassPtr.IsNull()) continue;

			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				RowMonsterInfo.MonsterSoftClassPtr.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda(
					[RowMonsterInfo, this]()
				{
					if (UClass* AsyncLoadMonsterClass = RowMonsterInfo.MonsterSoftClassPtr.Get())
					{
						AsyncLoadMonsterMap.Emplace(RowMonsterInfo.MonsterSoftClassPtr,AsyncLoadMonsterClass);
					}
				})
			);
		}
	}
}

void ADungeonGameMode::SpawnMonster()
{
	TArray<AActor*> MonsterSpawnPoints;
	
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), MonsterSpawnPoints);

	if (MonsterSpawnPoints.IsEmpty())
	{
		return;
	}
	
	TArray<UClass*> NormalMonsterClasses;
	
	TArray<FName> RowNames = MonsterDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FMonsterInDungeonTable* RowMonsterData = MonsterDataTable->FindRow<FMonsterInDungeonTable>(RowName, TEXT(""));
		if (RowMonsterData && RowMonsterData->MonsterType == EMonsterType::NormalMonster)
		{
			const FMonsterInfo& RowMonsterInfo = RowMonsterData->MonsterInfo;
			
			if (UClass** FoundClass = AsyncLoadMonsterMap.Find(RowMonsterInfo.MonsterSoftClassPtr))
			{
				NormalMonsterClasses.Add(*FoundClass);
			}
		}
	}

	if (NormalMonsterClasses.IsEmpty())
	{
		return;
	}

	int32 MonsterIndex = 0;
	for (AActor* SpawnPoint : MonsterSpawnPoints)
	{
		if (!SpawnPoint) continue;
		
		int32 SelectedMontserIndex = MonsterIndex % NormalMonsterClasses.Num();
		UClass* SelectedMonsterClass = NormalMonsterClasses[SelectedMontserIndex];

		FVector SpawnLocation = SpawnPoint->GetActorLocation();
		FRotator SpawnRotation = SpawnPoint->GetActorRotation();

		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this,SpawnLocation,RandomLocation,200.f);
		
		RandomLocation += FVector(0.f, 0.f, 100.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AMonsterCharacter* SpawnedMonster = GetWorld()->SpawnActor<AMonsterCharacter>(
			SelectedMonsterClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		MonsterIndex++;
	}
}
