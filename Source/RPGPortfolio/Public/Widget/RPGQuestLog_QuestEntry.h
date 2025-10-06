// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "RPGQuestLog_QuestEntry.generated.h"

class UButton;
class UCheckBox;
class UTextBlock;
class UScrollBox;
class ARPGQuestSystemActor;

//@ 퀘스트 선택될 시 이벤트 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestSelected, FName, QuestID, ARPGQuestSystemActor*, QuestActor);

//@ 퀘스트 트래킹 관련 이벤트 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestTracked, ARPGQuestSystemActor*, QuestActor, bool, IsCheck);
/**
 * URPGQuestLog_QuestEntry
 * 
 * 퀘스트 창에 쓰일 퀘스트 엔트리 위젯
 */
UCLASS()
class RPGPORTFOLIO_API URPGQuestLog_QuestEntry : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 퀘스트 선택될 시 이벤트 델리게이트 
	FOnQuestSelected OnQuestSelected;

	//@ 퀘스트 트래킹 관련 이벤트 델리게이트 
	FOnQuestTracked OnQuestTracked;

	//@ 퀘스트 ID 설정
	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDToSet);

	//@ 퀘스트 액터 설정
	UFUNCTION(BlueprintCallable)
	void SetQuestActor(ARPGQuestSystemActor* QuestActorToSet);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	//@ 퀘스트 트래킹 여부 체크박스 클릭 이벤트 핸들러
	UFUNCTION()
	void OnTrackCheckBoxClicked(bool IsCheck);

	//@ 퀘스트 선택 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnAcceptButtonClicked();

	//@ 선택 버튼
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UButton* SelectButton;

	// 트래킹 여부 체크박스
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UCheckBox* TrackCheckBox;

	//@ 퀘스트 이름 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	//@ 퀘스트 ID
	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FName QuestID;

	//@ 퀘스트 액터 참조
	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuestSystemActor* QuestActor;
};
