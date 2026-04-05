// LJS


#include "GameMode/GameManager/CombatManager.h"
#include "DataAsset/SaveGame/ParryingHistorySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"
#include "Containers/Queue.h"

DEFINE_LOG_CATEGORY_STATIC(LogParryCombat, Log, All);  // ← 추가

void UCombatManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RunBenchmark();

	LoadParryingData();


	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
	RecordParryAttempt(false);
	RecordParryAttempt(true);
}

void UCombatManager::Deinitialize()
{
	Super::Deinitialize();
}

// void UCombatManager::RecordParryAttempt(bool bSuccess)
// {
// 	// 순환 큐가 가득 찬 경우 가장 오래된 항목 제거
// 	if (TempParryResults.IsFull())
// 	{
// 		TempParryResults.Dequeue();
// 	}
// 	
// 	// 새로운 패링 결과 추가
// 	TempParryResults.Enqueue(bSuccess);
// 	
// 	SaveParryingData();
// }

void UCombatManager::RecordParryAttempt(bool bSuccess)
{
	if (TempParryResults.IsFull())
	{
		TempParryResults.Dequeue();
	}

	TempParryResults.Enqueue(bSuccess);

	SaveParryingData();

	// ↓ 추가 (SaveParryingData 이후여야 최신 수치 반영됨)
	if (IsValid(CurrentParryingData))
	{
		UE_VLOG(this, LogParryCombat, Log,
			TEXT("Parry %s | 성공률 : %.1f%% | 패링 판정 시간: %.3fs"),
			bSuccess ? TEXT("SUCCESS") : TEXT("FAIL"),
			CurrentParryingData->SuccessfulRate * 100.f,
			GetAdjustedCanParryingStateDelay());
	}
} 

float UCombatManager::GetAdjustedCanParryingStateDelay() const
{
	if (!IsValid(CurrentParryingData)) return BaseCanParryingStateTime;
	
	// 성공률 클램핑 (0.0~1.0)
	const float SuccessRate = FMath::Clamp(CurrentParryingData->SuccessfulRate, 0.0f, 1.0f);
	
	// 성공률에 따른 보너스 시간 계산 (성공률이 낮을수록 더 긴 시간)
	const float BonusTime = AddCanParryingStateDelayTime * (1.0f - SuccessRate);
    
	// 조정된 패링 판정 시간 계산
	const float AdjustedDelay = BaseCanParryingStateTime + BonusTime;

	return AdjustedDelay;
}

float UCombatManager::GetAdjustedParryingASCActiveTime() const
{
	// 패링 공격 및 패링 실패 판정에 쓸 시간, 패링 판정시간에 * 2
	return GetAdjustedCanParryingStateDelay() * 2;
}

void UCombatManager::SaveParryingData()
{
	if (!IsValid(CurrentParryingData) || TempParryResults.IsEmpty()) return;

	uint32 TotalAttempts = TempParryResults.Count();
	uint32 SuccessCount = 0;

	for (uint32 i = 0; i < TotalAttempts; ++i)
	{
		const bool* bSuccess = TempParryResults.AtIndex(i);
		if (bSuccess && *bSuccess)
		{
			SuccessCount++;
		}
	}

	if (TotalAttempts > 0)
	{
		const float CurrentSessionRate = (float)SuccessCount / (float)TotalAttempts;
		CurrentParryingData->SuccessfulRate = (SuccessAlpha * CurrentSessionRate) + ((1.0f - SuccessAlpha) * CurrentParryingData->SuccessfulRate);
	}

	UGameplayStatics::SaveGameToSlot(CurrentParryingData, CurrentParryingData->SaveSlotName, CurrentParryingData->UserIndex);
}

void UCombatManager::RunBenchmark()
{
	// 실제 게임과 동일한 윈도우 크기 (TCircularQueue 21 = 실효 슬롯 20)
	const int32 WindowSize   = 20;
	const int32 QueueCapacity = WindowSize + 1;
	const int32 Iterations   = 1'000'000;

	// bool(1바이트) 대신 실측 비용이 드러나는 64바이트 구조체 사용
	// → RemoveAt(0) 시 64바이트 × 20 = 1280바이트 이동, TCircularQueue와 비용 차이가 명확해짐
	struct FBenchRecord { uint8 Data[64]; };

	// -------------------------------------------------------
	// 1. TCircularQueue
	// -------------------------------------------------------
	double CQStart = FPlatformTime::Seconds();
	{
		TCircularQueue<FBenchRecord> CQ(QueueCapacity);
		FBenchRecord Rec{};
		for (int32 i = 0; i < Iterations; ++i)
		{
			if (CQ.IsFull()) CQ.Dequeue();
			CQ.Enqueue(Rec);
		}
	}
	const double CQTime = (FPlatformTime::Seconds() - CQStart) * 1000.0;
	const int32  CQAllocCount = 1;

	// -------------------------------------------------------
	// 2. TArray (슬라이딩 윈도우, RemoveAt(0) 방식)
	// -------------------------------------------------------
	double ArrStart = FPlatformTime::Seconds();
	{
		TArray<FBenchRecord> Arr;
		Arr.Reserve(WindowSize);
		FBenchRecord Rec{};
		for (int32 i = 0; i < Iterations; ++i)
		{
			// RemoveAt(0) = 뒤 원소 전부 앞으로 이동 (64바이트 × 20 = 1280바이트)
			if (Arr.Num() >= WindowSize) Arr.RemoveAt(0);
			Arr.Add(Rec);
		}
	}
	const double ArrTime     = (FPlatformTime::Seconds() - ArrStart) * 1000.0;
	const int32  ArrAllocCount = 1;
	const int32  ArrShiftOps   = FMath::Max(0, Iterations - WindowSize) * WindowSize;

	// -------------------------------------------------------
	// 3. TQueue (연결 리스트 기반, 매 노드 동적 할당)
	// -------------------------------------------------------
	double QStart = FPlatformTime::Seconds();
	{
		TQueue<FBenchRecord> Q;
		int32 QCount = 0;
		FBenchRecord Rec{};
		for (int32 i = 0; i < Iterations; ++i)
		{
			Q.Enqueue(Rec);   // 내부적으로 new TNode (heap malloc)
			QCount++;
			if (QCount > WindowSize)
			{
				FBenchRecord Dummy;
				Q.Dequeue(Dummy); // 내부적으로 delete TNode (heap free)
				QCount--;
			}
		}
	}
	const double QTime        = (FPlatformTime::Seconds() - QStart) * 1000.0;
	const int32  QAllocCount  = Iterations;
	const int32  QFreeCount   = FMath::Max(0, Iterations - WindowSize);
	const int32  QTotalHeapOps = QAllocCount + QFreeCount;
}

void UCombatManager::LoadParryingData()
{
	const FString SlotName = TEXT("ParryingData");
	const uint32 UserIndex = 0;
    
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		CurrentParryingData = Cast<UParryingHistorySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	}
	else
	{
		CurrentParryingData = Cast<UParryingHistorySaveGame>(UGameplayStatics::CreateSaveGameObject(UParryingHistorySaveGame::StaticClass()));
	}
}