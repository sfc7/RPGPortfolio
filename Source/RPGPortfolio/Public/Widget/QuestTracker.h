// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "GameMode/GameManager/QuestManager.h"
#include "QuestTracker.generated.h"

class UTextBlock;
class UVerticalBox;
struct FQuest;
class UButton;

/**
 * UQuestTracker
 * 
 * 퀘스트 트래킹 위젯, 체크박스로 설정 시에만 나오는 트래킹 기능
 */
UCLASS()
class RPGPORTFOLIO_API UQuestTracker : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	//@ 퀘스트 이름 텍스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	//@ 퀘스트 현재 오브젝트 보여주는 용 
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UVerticalBox* ObjectiveBox;

	//@ 현재 추적 중인 퀘스트 액터
	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuestSystemActor* QuestActor;

	//@ 오브젝트 엔트리 위젯 클래스 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestUI")
	TSubclassOf<UUserWidget> QuestEntry_ObjectiveWidgetClass;

	//@ 퀘스트 액터 업데이트
	void UpdateQuestActor(ARPGQuestSystemActor* QuestActorToSet);

	//@ 퀘스트 액터 설정 
	void SetQuestActor(ARPGQuestSystemActor* QuestActorToSet);

	//@ 퀘스트 완료 이벤트 핸들러
	UFUNCTION()
	void QuestCompleted(ARPGQuestSystemActor* QuestActorToSet);
	
	//@ 퀘스트 진행상황 델리게이트시 받을 함수
	UFUNCTION()
	void OnObjectiveHeard();
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;
};
