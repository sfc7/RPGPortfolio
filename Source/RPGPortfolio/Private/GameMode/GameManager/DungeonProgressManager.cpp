// LJS


#include "GameMode/GameManager/DungeonProgressManager.h"

#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/MonsterCharacter.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "GameMode/GameManager/UIManager.h"
#include "WorldStatic/SpawningVolume/SpawningVolume.h"
#include "GameMode/DungeonGameMode.h"
#include "Widget/RPGWidgetBase.h"
#include "Components/WidgetComponent.h"

UDungeonProgressManager::UDungeonProgressManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterSpawnTableRef(TEXT("/Script/Engine.DataTable'/Game/MyProject/System/GameMode/DataTable/DT_MonsterSpawnInDungeon.DT_MonsterSpawnInDungeon'"));
	if (MonsterSpawnTableRef.Succeeded())
	{
		MonsterSpawnTable = MonsterSpawnTableRef.Object;
	}
}

void UDungeonProgressManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (MonsterSpawnTable)
	{
		AsyncLoadSoftClassptrMonster();
	}

	OnDungeonStateChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnDungeonStateChanged);
}

void UDungeonProgressManager::Deinitialize()
{
	Super::Deinitialize();
}

void UDungeonProgressManager::SetDungeonState(EDungeonState GamemodeState)
{
	DungeonState = GamemodeState;

	OnDungeonStateChangedDelegate.Broadcast(GamemodeState);
}

void UDungeonProgressManager::AsyncLoadSoftClassptrMonster()
{
	if (!MonsterSpawnTable) return;

	TArray<FName> RowNames = MonsterSpawnTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		FMonsterSpawnTable* RowSpawnData = MonsterSpawnTable->FindRow<FMonsterSpawnTable>(RowName, TEXT(""));
		if (RowSpawnData)
		{
			const FMonsterInfo& RowMonsterInfo = RowSpawnData->MonsterInfo;

			if (RowMonsterInfo.MonsterSoftClassPtr.IsNull()) continue;

			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				RowMonsterInfo.MonsterSoftClassPtr.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda(
					[RowMonsterInfo, this]()
					{
						if (UClass* LoadedMonsterClass = RowMonsterInfo.MonsterSoftClassPtr.Get())
						{
							AsyncLoadMonsterMap.Emplace(RowMonsterInfo.MonsterSoftClassPtr, LoadedMonsterClass);
						}
					})
			);
		}
	}
}

void UDungeonProgressManager::SpawnMonster(ASpawningVolume& SpawningVolume)
{
	if (!MonsterSpawnTable) return;

	TArray<FName> RowNames = MonsterSpawnTable->GetRowNames();
	int32 MonsterIndex = 0;

	for (const FName& RowName : RowNames)
	{
		FMonsterSpawnTable* RowSpawnData = MonsterSpawnTable->FindRow<FMonsterSpawnTable>(RowName, TEXT("SpawnMonsterFromTable"));
		if (!RowSpawnData) continue;

		if (RowSpawnData->MonsterType != EMonsterType::NormalMonster) continue;

		const FMonsterInfo& RowMonsterInfo = RowSpawnData->MonsterInfo;

		if (UClass** FoundClass = AsyncLoadMonsterMap.Find(RowMonsterInfo.MonsterSoftClassPtr))
		{
			UClass* MonsterClass = *FoundClass;

			FVector SpawnLocation = RowSpawnData->SpawnLocation;
			FRotator SpawnRotation = RowSpawnData->SpawnRotation;

			AMonsterCharacter* SpawnedMonster = Cast<AMonsterCharacter>(SpawningVolume.SpawnActorAtBoxLocalLocation(MonsterClass, SpawnLocation));

			if (SpawnedMonster)
			{
				SpawnedMonster->OnDestroyed.AddUniqueDynamic(this, &ThisClass::MonsterKillCount);
				NormalMonsterCount++;
			}

			MonsterIndex++;
		}
	}
	
	SpawnFinish = true;
}

void UDungeonProgressManager::SpawnBossMonster(ASpawningVolume& SpawningVolume)
{
	TArray<FName> RowNames = MonsterSpawnTable->GetRowNames();
	int32 MonsterIndex = 0;

	for (const FName& RowName : RowNames)
	{
		FMonsterSpawnTable* RowSpawnData = MonsterSpawnTable->FindRow<FMonsterSpawnTable>(RowName, TEXT("SpawnMonsterFromTable"));
		if (!RowSpawnData) continue;

		if (RowSpawnData->MonsterType == EMonsterType::BossMonster)
		{
			const FMonsterInfo& RowMonsterInfo = RowSpawnData->MonsterInfo;

			if (UClass** FoundClass = AsyncLoadMonsterMap.Find(RowMonsterInfo.MonsterSoftClassPtr))
			{
				UClass* MonsterClass = *FoundClass;

				FVector SpawnLocation = RowSpawnData->SpawnLocation;
				FRotator SpawnRotation = RowSpawnData->SpawnRotation;

				AMonsterCharacter* SpawnedMonster = Cast<AMonsterCharacter>(SpawningVolume.SpawnActorAtBoxLocalLocation(MonsterClass, SpawnLocation));

				if (SpawnedMonster)
				{
					SpawnedMonster->OnDestroyed.AddUniqueDynamic(this, &ThisClass::BossMonsterKilled);

					if (SpawnedMonster->MonsterHpWidgetComponent)
					{
						URPGWidgetBase* HpWidget = Cast<URPGWidgetBase>(SpawnedMonster->MonsterHpWidgetComponent->GetUserWidgetObject());
						if (HpWidget)
						{
							HpWidget->InitMonsterCreatedWidget(SpawnedMonster);
						}
					}
				}
			}
		}
	}
}

void UDungeonProgressManager::RegisterSpawningVolume(ASpawningVolume* SpawningVolume)
{
	if (!SpawningVolume)
	{
		return;
	}

	FString SpawningVolumeName = SpawningVolume->GetVolumeName();
	if (SpawningVolumeName.IsEmpty())
	{
		return;
	}
	
	SpawningVolumes.Add(SpawningVolumeName, SpawningVolume);
}

ASpawningVolume* UDungeonProgressManager::FindSpawningVolumebyName(FString SpawningVolumeName) const
{
	if (SpawningVolumeName.IsEmpty())
	{
		return nullptr;
	}

	if (ASpawningVolume* const* FoundVolume = SpawningVolumes.Find(SpawningVolumeName))
	{
		return *FoundVolume;
	}

	return nullptr;
}

void UDungeonProgressManager::OnDungeonStateChanged(EDungeonState NewState)
{
	ADungeonGameMode* DungeonGameMode = Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(this));

	if (IsValid(DungeonGameMode))
	{
		switch (NewState)
		{
		case EDungeonState::NormalMonsterPhase:
			break;
            
		case EDungeonState::BossMonsterPhase:
			DungeonGameMode->DungeonCinemaPlay();			
			break;
            
		case EDungeonState::Clear:
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			GetGameInstance()->GetSubsystem<UUIManager>()->ShowUIAsync(EUICategory::VictoryUI, GetWorld());
			break;
            
		case EDungeonState::GameOver:
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			GetGameInstance()->GetSubsystem<UUIManager>()->ShowUIAsync(EUICategory::GameOverUI, GetWorld());
			break;
            
		default:
			break;
		}
	}

}

void UDungeonProgressManager::MonsterKillCount(AActor* DestroyedActor)
{
	NormalMonsterCount--;
	
	if (SpawnFinish && NormalMonsterCount == 0)
	{
		SetDungeonState(EDungeonState::BossMonsterPhase);
	}
}

void UDungeonProgressManager::BossMonsterKilled(AActor* DestroyedActor)
{
	SetDungeonState(EDungeonState::Clear);
}

