// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "GameMode/GameManager/QuestManager.h"
#include "QuestWidget.generated.h"


class UButton;
class UTextBlock;
struct FQuest;
class UItemSlotContainer;

/**
 * UQuestWidget
 * 
 * 퀘스트 UI 위젯
 */
UCLASS()
class RPGPORTFOLIO_API UQuestWidget : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 퀘스트 정보 가져오기
	UFUNCTION(BlueprintCallable)
	FQuest GetQuest() const { return QuestDetails; }

	//@ 퀘스트 정보 설정
	UFUNCTION(BlueprintCallable)
	void SetQuest(FQuest QuestDetailsToSet);

	//@ 퀘스트 ID 설정
	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDToSet);
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	//@ 수락 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	virtual void OnAcceptButtonClicked();

	//@ 보상 아이템 슬롯 생성
	UFUNCTION(BlueprintCallable)
	void CreateRewardItemSlots();

	//@ 아이템 슬롯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemSlotMasterClass;

	//@ 퀘스트 이름 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	//@ 퀘스트 설명 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestDescription;

	//@ 단계 설명 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* StageDescription;

	//@ 보상 아이템 컨테이너
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UItemSlotContainer* RewardItemContainer;

	//@ 수락 버튼
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UButton* AcceptButton;

	//@ 거절 버튼
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))	
	UButton* DeclineButton;

	//@ 퀘스트 상세 정보
	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FQuest QuestDetails;

	//@ 퀘스트 골드 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestGold;

	//@ 퀘스트 ID
	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FName QuestID;
};
