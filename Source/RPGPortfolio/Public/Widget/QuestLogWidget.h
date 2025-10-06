// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "QuestLogWidget.generated.h"

class UScrollBox;
class UWidgetSwitcher;
class UTextBlock;
class UVerticalBox;
class ARPGQuestSystemActor;
class UItemSlotContainer;
struct FQuest;
class UQuestTracker;

/**
 * UQuestLogWidget
 * 
 * 퀘스트 전체 창을 보여줄 위젯
 *
 */
UCLASS()
class RPGPORTFOLIO_API UQuestLogWidget : public URPGWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	//@ 퀘스트 선택 이벤트 핸들러
	UFUNCTION()
	void OnQuestSelected(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet);

	//@ 퀘스트 트래킹 이벤트 핸들러
	UFUNCTION()
	void OnQuestTracked(ARPGQuestSystemActor* QuestActorToSet, bool IsCheck);

	//@ 퀘스트 상세 정보 표시
	UFUNCTION()
	void DisplayQuestToRightDetail(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet);

	//@ 보상 아이템 슬롯 생성
	UFUNCTION(BlueprintCallable)
	void CreateRewardItemSlots(FQuest QuestDetail);

	//@ 아이템 슬롯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemSlotMasterClass;

	//@ 퀘스트 이름 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	//@ 퀘스트 엔트리 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Quest_LogEntryWidgetClass;

	//@ 퀘스트 오브젝트 엔트리 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> LogEntry_ObjectiveWidgetClass;

	//@ 퀘스트 트래킹 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Quest_TrackWidgetClass;

	//@ 퀘스트 추적 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestTracker* TrackWidget;

	//@ 퀘스트 스테이지들 설명
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* StageDescription;

	//@ 전체적인 퀘스트 설명
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestDescription;

	//@ 퀘스트 보상 골드
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestGold;

	//@ 스크롤바
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UScrollBox* QuestScrollBox;

	//@ 보상 아이템 슬롯 컨테이너
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UItemSlotContainer* RewardItemContainer;

	//@ 퀘스트 선택하면 오른쪽 화면에 나오는 퀘스트 정보 (Switcher로 구현)
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UWidgetSwitcher* RightQuestDetailWidgetSwitcher;	

	//@ 퀘스트 오브젝트 내용에 쓸 VerticalBox
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UVerticalBox* ObjectiveVerticalBox;

	//@ 현재 퀘스트 액터
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI")
	ARPGQuestSystemActor* CurrentQuestActor;
};
