// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGUIData.generated.h"

UENUM(BlueprintType)
enum class EUICategory : uint8
{
	GameOverUI,
	VictoryUI,
	PauseMenuUI,
	TitleMenuUI,
	None,
};

UENUM(BlueprintType)
enum class ELoadingCategory : uint8
{
	Town,
	Dungeon,
	None,
};

USTRUCT(BlueprintType)
struct FUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EUICategory UIType = EUICategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> UIWidgetClass;
};

USTRUCT(BlueprintType)
struct FLoadingScreenData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ELoadingCategory LoadingMap = ELoadingCategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UUserWidget> LoadingScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FGameplayTag MapTag;
};
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGUIData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGUIData(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetUIWidgetClass(const EUICategory& UICategory) const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLoadingScreen(const ELoadingCategory& LoadingCategory) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData VictoryUI;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData GameOverUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData PauseMenuUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData TitleMenuUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FLoadingScreenData LoadingScreen_Town;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FLoadingScreenData LoadingScreen_Dungeon;
};
