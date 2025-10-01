// LJS


#include "GameMode/GameManager/LevelManager.h"

#include "DataAsset/DataAsset_RPGLevelData.h"
#include "DataAsset/SaveGame/RPGSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Component/InventoryComponent.h"
#include "Component/Player/PlayerInventoryComponent.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/UIManager.h"
#include "Quest/RPGQuestSystemActor.h"
ULevelManager::ULevelManager()
{
	
}

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
	RPGLevelData = LoadObject<UDataAsset_RPGLevelData>(nullptr, 
		TEXT("/Game/MyProject/Data/DA_RPGLevelData.DA_RPGLevelData"));
}

void ULevelManager::Deinitialize()
{
	Super::Deinitialize();
}

TSoftObjectPtr<UWorld> ULevelManager::GetGameLevelByTag(FGameplayTag GameplayTag) const
{
	return RPGLevelData->GetGameLevelByTag(GameplayTag);
}

void ULevelManager::SaveRPGGame()
{
	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass());

	if (URPGSaveGame* RpgSaveGame = Cast<URPGSaveGame>(SaveGame))
	{
		// 퀘스트 데이터 저장
		TArray<ARPGQuestSystemActor*> CurrentQuests = GetGameInstance()->GetSubsystem<UQuestManager>()->GetCurrentQuests();
		for (ARPGQuestSystemActor* CurrentQuest : CurrentQuests)
		{
			RpgSaveGame->SaveQuestDetails(CurrentQuest);
		}
		UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();
		if (QuestManager)
		{
			RpgSaveGame->SetCurrentActiveQuests(QuestManager->GetCurrentActiveQuests());
			RpgSaveGame->SetCompletedQuests(QuestManager->GetCompletedQuests());
		}

		// 플레이어 데이터 저장
		SavePlayerInventoryData(RpgSaveGame);
		SavePlayerGold(RpgSaveGame);
		SavePlayerAttribute(RpgSaveGame);
		
		UGameplayStatics::SaveGameToSlot(RpgSaveGame, TEXT("RPGSaveSlot"), 1);
	}
}

void ULevelManager::LoadRPGGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("RPGSaveSlot"), 1))
	{
		USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("RPGSaveSlot"), 1);
        
		if (URPGSaveGame* RpgSaveGame = Cast<URPGSaveGame>(LoadedGame))
		{
			// 퀘스트 데이터 로드
			UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();

			QuestManager->ClearAllQuests();

			//퀘스트 액터 스폰, 로드 적용 (현재 퀘스트는 액터단위로 구현됨)
			TArray<FName> SavedActiveQuests = RpgSaveGame->GetCurrentActiveQuests();
			for (FName CurrentActiveQuest : SavedActiveQuests)
			{
				ARPGQuestSystemActor* AddQuest = QuestManager->AddNewQuest(CurrentActiveQuest);
				if (AddQuest)
				{
					TMap<FName, FQuestSaveData> QuestProgress = RpgSaveGame->GetQuestProgress();
					FQuestSaveData* FindQuestSaveData = QuestProgress.Find(CurrentActiveQuest);
					
					if (FindQuestSaveData)
					{
						AddQuest->SetCurrentStage(FindQuestSaveData->CurrentStage);
						AddQuest->SetCurrentObjectiveProgress(FindQuestSaveData->QuestProgress);
					}
				}
			}
            
			QuestManager->SetCurrentActiveQuests(SavedActiveQuests);
			QuestManager->SetCompleteActiveQuests(RpgSaveGame->GetCompletedQuests());

			// 플레이어 데이터 로드
			LoadPlayerInventoryData(RpgSaveGame);
			LoadPlayerGold(RpgSaveGame);

			LoadPlayerAttribute(RpgSaveGame);
		}
	}
}

void ULevelManager::SavePlayerInventoryData(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	APlayerCharacterBase* const Player = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!IsValid(Player)) return;
	
	// 플레이어 인벤토리 컴포넌트 저장
	if (Player->GetPlayerInventoryComponent())
	{
		SaveGame->SetPlayerInventorySlots(Player->GetPlayerInventoryComponent()->DefaultItemSlots);
	}

	// 플레이어 포션 퀵슬롯 저장
	if (Player->GetPlayerPotionHotBar())
	{
		SaveGame->SetPlayerPotionSlots(Player->GetPlayerPotionHotBar()->DefaultItemSlots);
	}
}

void ULevelManager::LoadPlayerInventoryData(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;
	
	APlayerCharacterBase* const Player = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!IsValid(Player)) return;

	// 플레이어 인벤토리 컴포넌트 로드
	if (Player->GetPlayerInventoryComponent())
	{
		TArray<FInventorySlot> LoadedItemSlots = SaveGame->GetPlayerInventorySlots();
		Player->GetPlayerInventoryComponent()->DefaultItemSlots = LoadedItemSlots;
		
		for (int32 i = 0; i < LoadedItemSlots.Num(); ++i)
		{
			Player->GetPlayerInventoryComponent()->DefaultItemSlots[i].SlotIndex = i;
			Player->GetPlayerInventoryComponent()->DefaultItemSlots[i].InventoryRef = Player->GetPlayerInventoryComponent();
		}
	}
	// 플레이어 포션 퀵슬롯 로드
	if (Player->GetPlayerPotionHotBar())
	{
		TArray<FInventorySlot> LoadedPotionSlots = SaveGame->GetPlayerPotionSlots();
		Player->GetPlayerPotionHotBar()->DefaultItemSlots = LoadedPotionSlots;
		
		for (int32 i = 0; i < LoadedPotionSlots.Num(); ++i)
		{
			Player->GetPlayerPotionHotBar()->DefaultItemSlots[i].SlotIndex = i;
			Player->GetPlayerPotionHotBar()->DefaultItemSlots[i].InventoryRef = Player->GetPlayerPotionHotBar();
		}
		
		Player->GetPlayerPotionHotBar()->OnPotionBarSlotChangedDelegate.Broadcast();
	}
}

void ULevelManager::SavePlayerGold(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController)) return;

	APlayerCharacterBase* const Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!IsValid(Player)) return;

	// 플레이어 골드 저장
	if (Player->GetPlayerInventoryComponent())
	{
		SaveGame->SetPlayerGold(Player->GetPlayerInventoryComponent()->GetPlayerGold());
	}
}

void ULevelManager::LoadPlayerGold(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController)) return;

	APlayerCharacterBase* const Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!IsValid(Player)) return;

	// 플레이어 골드 로드
	if (Player->GetPlayerInventoryComponent())
	{
		int32 LoadedPlayerGold = SaveGame->GetPlayerGold();
		Player->GetPlayerInventoryComponent()->SetGold(LoadedPlayerGold);
	}
}

void ULevelManager::SavePlayerAttribute(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController)) return;

	APlayerCharacterBase* const Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!IsValid(Player)) return;

	// 플레이어 GAS용 AttributeSet 데이터 저장
	TArray<FAttributeSaveData> AttributeData = Player->GetRPGAttributeSet()->SaveAllAttributesToSaveData();
	SaveGame->SetSavedAttributes(AttributeData);	
}

void ULevelManager::LoadPlayerAttribute(URPGSaveGame* SaveGame)
{
	if (!IsValid(SaveGame)) return;

	APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController)) return;

	APlayerCharacterBase* const Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!IsValid(Player)) return;

	// 플레이어 GAS용 AttributeSet 데이터 로드
	TArray<FAttributeSaveData> AttributeData = SaveGame->GetSavedAttributes();
	Player->GetRPGAttributeSet()->LoadAllAttributesFromSaveData(AttributeData);
	
}

void ULevelManager::LoadLoadingScreen(const FString& LevelName)
{
	// 로딩 스크린 속성 설정
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;	
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void ULevelManager::DestinationLoadingScreen(UWorld* LoadWorld)
{
	// 로딩 스크린 종료
	GetMoviePlayer()->StopMovie();
}
