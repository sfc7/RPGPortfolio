// LJS

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_RPGUIData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGStructTypes.h"
#include "UIManager.generated.h"

class UDataAsset_RPGUIData;
class UInventoryComponent;

/**
 * UUIManager
 * 
 * UI 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UUIManager();

	//@ UI 표시 (비동기식으로 함)
	UFUNCTION(BlueprintCallable)
	void ShowUIAsync(const EUICategory& UICategory, UWorld* World);

	//@ UI 카테고리별 위젯 클래스 가져오기
	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetUIWidgetClass(const EUICategory& UICategory) const;

	//@ 로딩 스크린 위젯 클래스 가져오기
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLoadingScreen(const ELoadingCategory& LoadingCategory) const;

	//@ 인벤토리 UI 위젯 클래스 가져오기 
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetInventoryUIWidgetClass(const EInventoryUICategory& InventoryUICategory) const;

	//@ 상호작용 UI 위젯 클래스 가져오기 
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetInteractWidgetClass(const EInteractUICategory& InteractUICategory) const;

	//@ 퀘스트 UI 위젯 클래스 가져오기
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetQuestWidgetClass(const EQuestUICategory& QuestUICategory) const;

	//@ UI 입력 모드 전환
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, ERPGInputMode InputMode);

	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	//@ UI 캐싱 테이블
	UPROPERTY()
	UDataAsset_RPGUIData* RPGUIData;
};
