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
			UQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UQuestManager>();

			QuestManager->ClearAllQuests();
			
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

			LoadPlayerInventoryData(RpgSaveGame);
			LoadPlayerGold(RpgSaveGame);

			LoadPlayerAttribute(RpgSaveGame);
		}
	}
}

void ULevelManager::SavePlayerInventoryData(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;

	APlayerCharacterBase* Player =  Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!Player)
	{
		return;
	}

	if (Player->GetPlayerInventoryComponent())
	{
		SaveGame->SetPlayerInventorySlots(Player->GetPlayerInventoryComponent()->DefaultItemSlots);
	}

	if (Player->GetPlayerPotionHotBar())
	{
		SaveGame->SetPlayerPotionSlots(Player->GetPlayerPotionHotBar()->DefaultItemSlots);
	}
}

void ULevelManager::LoadPlayerInventoryData(URPGSaveGame* SaveGame)
{
	if (!SaveGame) return;
	
	APlayerCharacterBase* Player =  Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player && Player->GetPlayerInventoryComponent())
	{
		TArray<FInventorySlot> LoadedItemSlots = SaveGame->GetPlayerInventorySlots();
		Player->GetPlayerInventoryComponent()->DefaultItemSlots = LoadedItemSlots;
		
		for (int32 i = 0; i < LoadedItemSlots.Num(); ++i)
		{
			Player->GetPlayerInventoryComponent()->DefaultItemSlots[i].SlotIndex = i;
			Player->GetPlayerInventoryComponent()->DefaultItemSlots[i].InventoryRef = Player->GetPlayerInventoryComponent();
		}
	}

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
	if (!SaveGame)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!Player)
	{
		return;
	}
	
	if (Player && Player->GetPlayerInventoryComponent())
	{
		SaveGame->SetPlayerGold(Player->GetPlayerInventoryComponent()->GetPlayerGold());
	}
}

void ULevelManager::LoadPlayerGold(URPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!Player)
	{
		return;
	}
	
	if (Player && Player->GetPlayerInventoryComponent())
	{
		int32 LoadedPlayerGold = SaveGame->GetPlayerGold();
		Player->GetPlayerInventoryComponent()->SetGold(LoadedPlayerGold);
	}
}

void ULevelManager::SavePlayerAttribute(URPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!Player)
	{
		return;
	}
	
	if (Player)
	{
		TArray<FAttributeSaveData> AttributeData = Player->GetRPGAttributeSet()->SaveAllAttributesToSaveData();
		SaveGame->SetSavedAttributes(AttributeData);
	}
}

void ULevelManager::LoadPlayerAttribute(URPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(PlayerController->GetPawn());
	if (!Player)
	{
		return;
	}

	if (Player)
	{
		TArray<FAttributeSaveData> AttributeData = SaveGame->GetSavedAttributes();
		Player->GetRPGAttributeSet()->LoadAllAttributesFromSaveData(AttributeData);
	}
}

void ULevelManager::LoadLoadingScreen(const FString& LevelName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;	
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void ULevelManager::DestinationLoadingScreen(UWorld* LoadWorld)
{
	GetMoviePlayer()->StopMovie();
}
