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

UDungeonProgressManager::UDungeonProgressManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableRef(TEXT("/Script/Engine.DataTable'/Game/MyProject/System/GameMode/DataTable/DT_MonsterInDuegeon.DT_MonsterInDuegeon'"));
	if (MonsterDataTableRef.Succeeded())
	{
		MonsterDataTable = MonsterDataTableRef.Object;
	}
}

void UDungeonProgressManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (MonsterDataTable)
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

void UDungeonProgressManager::SpawnMonster()
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

		SpawnedMonster->OnDestroyed.AddUniqueDynamic(this, &ThisClass::MonsterKillCount);
		NormalMonsterCount++;
		MonsterIndex++;
	}

	SpawnFinish = true;
}

void UDungeonProgressManager::OnDungeonStateChanged(EDungeonState NewState)
{
	switch (NewState)
	{
	case EDungeonState::NormalMonsterPhase:

		break;
            
	case EDungeonState::BossMonsterPhase:

		break;
            
	case EDungeonState::Clear:

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
		SetDungeonState(EDungeonState::BossMonsterPhase);
	}
}
