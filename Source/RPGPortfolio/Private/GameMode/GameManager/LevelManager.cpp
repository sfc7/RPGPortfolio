// LJS


#include "GameMode/GameManager/LevelManager.h"

#include "DataAsset/DataAsset_RPGLevelData.h"
#include "DataAsset/SaveGame/RPGSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/UIManager.h"

ULevelManager::ULevelManager()
{
	
}

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
	RPGLevelData = LoadObject<UDataAsset_RPGLevelData>(nullptr, 
		TEXT("/Game/MyProject/Data/DA_RPGLevelData.DA_RPGLevelData"));

	// FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::LoadLoadingScreen);
	// FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::DestinationLoadingScreen);
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
		bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(RpgSaveGame, TEXT("PlayerCharacterSlot"), 1);
	}
}

void ULevelManager::LoadRPGGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("RPGSaveSlot"), 1))
	{
		USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("RPGSaveSlot"), 1);
        
		if (URPGSaveGame* RpgSaveGame = Cast<URPGSaveGame>(LoadedGame))
		{
			
		}
	}
}

void ULevelManager::LoadLoadingScreen(const FString& LevelName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;	

	// TSubclassOf<UUserWidget> LoadingScreenClass;
	// UUserWidget* LoadingWidget = nullptr;
	//
	// if (LevelName == TEXT("Town"))
	// {
	// 	LoadingScreenClass = GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->GetLoadingScreen(ELoadingCategory::Town);
	// }
	// else if (LevelName == TEXT("Dungeon"))
	// {
	// 	LoadingScreenClass = GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetUIManager()->GetLoadingScreen(ELoadingCategory::Dungeon);
	// }
	//
	// if (LoadingScreenClass)
	// {
	// 	LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
	// 	if (LoadingWidget)
	// 	{
	// 		LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	// 	}
	// }
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void ULevelManager::DestinationLoadingScreen(UWorld* LoadWorld)
{
	GetMoviePlayer()->StopMovie();
}
