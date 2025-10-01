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
	// 몬스터 스폰 테이블 로드
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterSpawnTableRef(TEXT("/Script/Engine.DataTable'/Game/MyProject/System/GameMode/DataTable/DT_MonsterSpawnInDungeon.DT_MonsterSpawnInDungeon'"));
	if (MonsterSpawnTableRef.Succeeded())
	{
		MonsterSpawnTable = MonsterSpawnTableRef.Object;
	}
}

void UDungeonProgressManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IsValid(MonsterSpawnTable))
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
	// 던전 상태 업데이트
	DungeonState = GamemodeState;

	OnDungeonStateChangedDelegate.Broadcast(GamemodeState);
}

void UDungeonProgressManager::AsyncLoadSoftClassptrMonster()
{
	if (!IsValid(MonsterSpawnTable)) return;

	// 데이터 테이블의 모든 행 이름 가져오기
	TArray<FName> RowNames = MonsterSpawnTable->GetRowNames();

	// 각 행에 대해 몬스터 클래스 비동기 로딩
	for (const FName& RowName : RowNames)
	{
		FMonsterSpawnTable* RowSpawnData = MonsterSpawnTable->FindRow<FMonsterSpawnTable>(RowName, TEXT(""));
		if (RowSpawnData)
		{
			const FMonsterInfo& RowMonsterInfo = RowSpawnData->MonsterInfo;

			if (RowMonsterInfo.MonsterSoftClassPtr.IsNull()) continue;

			// 비동기 로딩 요청
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
	if (!IsValid(MonsterSpawnTable)) return;

	// 데이터 테이블의 모든 행 이름 가져오기
	TArray<FName> RowNames = MonsterSpawnTable->GetRowNames();
	int32 MonsterIndex = 0;

	// 각 행에 대해 일반 몬스터 스폰
	for (const FName& RowName : RowNames)
	{
		FMonsterSpawnTable* RowSpawnData = MonsterSpawnTable->FindRow<FMonsterSpawnTable>(RowName, TEXT("SpawnMonsterFromTable"));
		if (!RowSpawnData) continue;

		// 일반 몬스터 타입인지 확인
		if (RowSpawnData->MonsterType != EMonsterType::NormalMonster) continue;

		const FMonsterInfo& RowMonsterInfo = RowSpawnData->MonsterInfo;
		
		// 비동기 로드된 몬스터 클래스 찾아서 스폰
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
	if (!IsValid(MonsterSpawnTable)) return;
	
	// 데이터 테이블의 모든 행 이름 가져오기
	TArray<FName> RowNames = MonsterSpawnTable->GetRowNames();
	int32 MonsterIndex = 0;

	// 각 행에 대해 보스 몬스터 스폰
	for (const FName& RowName : RowNames)
	{
		FMonsterSpawnTable* RowSpawnData = MonsterSpawnTable->FindRow<FMonsterSpawnTable>(RowName, TEXT("SpawnMonsterFromTable"));
		if (!RowSpawnData) continue;

		// 보스 몬스터 타입인지 확인
		if (RowSpawnData->MonsterType != EMonsterType::BossMonster) continue;
		
		const FMonsterInfo& RowMonsterInfo = RowSpawnData->MonsterInfo;

		// 비동기 로드된 몬스터 클래스 찾아서 스폰
		if (UClass** FoundClass = AsyncLoadMonsterMap.Find(RowMonsterInfo.MonsterSoftClassPtr))
		{
			UClass* MonsterClass = *FoundClass;

			FVector SpawnLocation = RowSpawnData->SpawnLocation;
			FRotator SpawnRotation = RowSpawnData->SpawnRotation;

			AMonsterCharacter* SpawnedMonster = Cast<AMonsterCharacter>(SpawningVolume.SpawnActorAtBoxLocalLocation(MonsterClass, SpawnLocation));

			if (SpawnedMonster)
			{
				SpawnedMonster->OnDestroyed.AddUniqueDynamic(this, &ThisClass::BossMonsterKilled);

				// 보스몬스터 관련 HUD 처리
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

void UDungeonProgressManager::RegisterSpawningVolume(ASpawningVolume* SpawningVolume)
{
	if (!IsValid(SpawningVolume)) return;

	// 스폰 볼륨 이름 가져와서 맵에 추가
	FString SpawningVolumeName = SpawningVolume->GetVolumeName();
	if (SpawningVolumeName.IsEmpty())
	{
		return;
	}
	
	SpawningVolumes.Add(SpawningVolumeName, SpawningVolume);
}

ASpawningVolume* UDungeonProgressManager::FindSpawningVolumebyName(FString SpawningVolumeName) const
{
	if (SpawningVolumeName.IsEmpty()) return nullptr;

	// 스폰 볼륨 맵에서 이름 찾기
	if (ASpawningVolume* const* FoundVolume = SpawningVolumes.Find(SpawningVolumeName))
	{
		return *FoundVolume;
	}

	return nullptr;
}

void UDungeonProgressManager::OnDungeonStateChanged(EDungeonState NewState)
{
	ADungeonGameMode* const DungeonGameMode = Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(DungeonGameMode)) return;

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

void UDungeonProgressManager::MonsterKillCount(AActor* DestroyedActor)
{
	NormalMonsterCount--;
	
	if (SpawnFinish && NormalMonsterCount == 0)
	{
		// 스폰 완료이고 모든 일반 몬스터가 죽었으면 보스 페이즈로 전환
		SetDungeonState(EDungeonState::BossMonsterPhase);
	}
}

void UDungeonProgressManager::BossMonsterKilled(AActor* DestroyedActor)
{
	// 보스 몬스터 처리시 해당 던전 클리어 
	SetDungeonState(EDungeonState::Clear);
}

