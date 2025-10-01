// LJS


#include "GameMode/GameManager/CombatManager.h"
#include "DataAsset/SaveGame/ParryingHistorySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"

void UCombatManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadParryingData();
}

void UCombatManager::Deinitialize()
{
	Super::Deinitialize();
}

void UCombatManager::RecordParryAttempt(bool bSuccess)
{
	// 순환 큐가 가득 찬 경우 가장 오래된 항목 제거
	if (TempParryResults.IsFull())
	{
		TempParryResults.Dequeue();
	}
	
	// 새로운 패링 결과 추가
	TempParryResults.Enqueue(bSuccess);
	
	SaveParryingData();
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

	// 총 시도 횟수 및 성공 횟수 계산
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

	// 시도 횟수가 0보다 큰 경우에만 처리
	if (TotalAttempts > 0)
	{
		// 현재 세션 성공률 계산
		const float CurrentSessionRate = (float)SuccessCount / (float)TotalAttempts;
		
		// 가중 평균으로 성공률 업데이트
		CurrentParryingData->SuccessfulRate = (SuccessAlpha * CurrentSessionRate) + ((1.0f - SuccessAlpha) * CurrentParryingData->TotalSuccessfulRate);

		// 기록 횟수가 ResetCount가 되면 누적 성공률 갱신	
		CurrentParryingData->RecordCount++;
		if (CurrentParryingData->ResetCount == CurrentParryingData->RecordCount)
		{
			CurrentParryingData->RecordCount = 0;
			CurrentParryingData->TotalSuccessfulRate = CurrentParryingData->SuccessfulRate;
		}
	}

	UGameplayStatics::SaveGameToSlot(CurrentParryingData, CurrentParryingData->SaveSlotName, CurrentParryingData->UserIndex);
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