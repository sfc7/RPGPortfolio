// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGUIData.generated.h"

//@ UI 타입들
UENUM(BlueprintType)
enum class EUICategory : uint8
{
	GameOverUI,
	VictoryUI,
	PauseMenuUI,
	TitleMenuUI,
	InventoryUI,
	QuestUI,
	SkillUI,
	None,
};

//@ 로딩 화면 카테고리 - 레벨별 로딩 화면 구분
UENUM(BlueprintType)
enum class ELoadingCategory : uint8
{
	Town,
	Dungeon,
	None,
};

//@ 인벤토리 슬롯 UI 
UENUM(BlueprintType)
enum class EInventoryUICategory : uint8
{
	ItemSlot,
	PotionSlot,
	None,
};

//@ 상호작용 UI 카테고리
UENUM(BlueprintType)
enum class EInteractUICategory : uint8
{
	Storage,
	Store,
	None,
};

//@ 퀘스트 UI 카테고리
UENUM(BlueprintType)
enum class EQuestUICategory : uint8
{
	NPC,
	Reward,
	None
};

//@ UI 데이터 구조체 - UI 타입과 위젯 클래스 매핑
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

//@ 로딩 화면 데이터 구조체 - 로딩 화면에 따라 UI,FGameplayTag 매핑
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

//@ 인벤토리 UI 데이터 구조체 - 인벤토리 타입에 따라 UI 매핑
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

//@ 상호작용 UI 데이터 구조체 - NPC 상호작용에 따라 UI 매핑
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

//@ 퀘스트 UI 데이터 구조체 - 퀘스트 타입에 따라 UI 매핑
USTRUCT(BlueprintType)
struct FQuestUI
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestUICategory QuestUIType = EQuestUICategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InteractUIWidgetClass;
};
/**
 * UDataAsset_RPGUIData
 * 
 * UI 관리 데이터 애셋
 * 모든 게임 UI 위젯 클래스들의 중앙 집중식 관리, UI 매니저 시스템과 연동
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGUIData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGUIData(const FObjectInitializer& ObjectInitializer);

	//@ UI 카테고리로 해당 위젯 클래스 검색
	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetUIWidgetClass(const EUICategory& UICategory) const;

	//@ 로딩 카테고리로 해당 로딩 화면 검색
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLoadingScreen(const ELoadingCategory& LoadingCategory) const;

	//@ 인벤토리 UI 카테고리로 해당 위젯 클래스 검색
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetInventoryUIWidgetClass(const EInventoryUICategory& InventoryUICategory) const;

	//@ 상호작용 UI 카테고리로 해당 위젯 클래스 검색
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetInteractUIWidgetClass(const EInteractUICategory& InteractUICategory) const;

	//@ 퀘스트 UI 카테고리로 해당 위젯 클래스 검색
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FUIData PlayerQuestUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FUIData PlayerSkillUI;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InteractUI")
	FInteractUI StoreUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestUI")
	FQuestUI NPCQuestUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestUI")
	FQuestUI NPCQuesRewardsUI;
	
};
