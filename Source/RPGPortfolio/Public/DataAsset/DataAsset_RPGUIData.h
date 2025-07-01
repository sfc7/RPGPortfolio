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
	InventoryUI,
	None,
};

UENUM(BlueprintType)
enum class ELoadingCategory : uint8
{
	Town,
	Dungeon,
	None,
};

UENUM(BlueprintType)
enum class EInventoryUICategory : uint8
{
	ItemSlot,
	PotionSlot,
	None,
};

UENUM(BlueprintType)
enum class EInteractUICategory : uint8
{
	Storage,
	None,
};

UENUM(BlueprintType)
enum class EQuestUICategory : uint8
{
	NPC,
	None
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

USTRUCT(BlueprintType)
struct FInventoryUI
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EInventoryUICategory InventoryUIType = EInventoryUICategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventoryUIWidgetClass;
};

USTRUCT(BlueprintType)
struct FInteractUI
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EInteractUICategory InteractUIType = EInteractUICategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InteractUIWidgetClass;
};

USTRUCT(BlueprintType)
struct FQuestUI
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestUICategory QuestUIType = EQuestUICategory::NPC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InteractUIWidgetClass;
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

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetInventoryUIWidgetClass(const EInventoryUICategory& InventoryUICategory) const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetInteractUIWidgetClass(const EInteractUICategory& InteractUICategory) const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetQuestUIWidgetClass(const EQuestUICategory& QuestUICategory) const;
	
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
	FUIData InventoryUI;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LoadingScreenData")
	FLoadingScreenData LoadingScreen_Town;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LoadingScreenData")
	FLoadingScreenData LoadingScreen_Dungeon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventoryUI")
	FInventoryUI ItemSlotUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventoryUI")
	FInventoryUI PotionSlotUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InteractUI")
	FInteractUI StorageUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestUI")
	FQuestUI NPCQuestUI;


	
};
