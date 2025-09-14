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

	if (CurrentParryingData)
	{
		SaveParryingData();
	}
}

void UCombatManager::RecordParryAttempt(bool bSuccess)
{
	if (TempParryResults.IsFull())
	{
		bool OldValue;
		TempParryResults.Dequeue(OldValue);
	}
	
	bool bEnqueued = TempParryResults.Enqueue(bSuccess);

	SaveParryingData();
}

float UCombatManager::GetAdjustedCanParryingStateDelay() const
{
	if (!CurrentParryingData) return BaseCanParryingStateDelayTime;
	
	float SuccessRate = FMath::Clamp(CurrentParryingData->SuccessfulRate, 0.0f, 1.0f);
	float BonusTime = AddCanParryingStateDelayTime * (1.0f - SuccessRate);
    
	float AdjustedDelay = BaseCanParryingStateDelayTime + BonusTime;

	return AdjustedDelay;
}

float UCombatManager::GetAdjustedParryingASCActiveTime() const
{
	return GetAdjustedCanParryingStateDelay() * 2;
}

void UCombatManager::SaveParryingData()
{
	if (!CurrentParryingData || TempParryResults.IsEmpty()) 
	{
		return;
	}
	
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
		float CurrentSessionRate = (float)SuccessCount / (float)TotalAttempts;
		
		//PreviousSessionAlpha : 이전 성공률을 반영할지(옵션)
		CurrentParryingData->SuccessfulRate = (PreviousSessionAlpha * CurrentSessionRate) + ((1.0f - PreviousSessionAlpha) * CurrentParryingData->SuccessfulRate);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentParryingData, CurrentParryingData->SaveSlotName, CurrentParryingData->UserIndex);
}

void UCombatManager::LoadParryingData()
{
	FString SlotName = TEXT("ParryingData");
	uint32 UserIndex = 0;
    
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		CurrentParryingData = Cast<UParryingHistorySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	}
	else
	{
		CurrentParryingData = Cast<UParryingHistorySaveGame>(UGameplayStatics::CreateSaveGameObject(UParryingHistorySaveGame::StaticClass()));
	}
}