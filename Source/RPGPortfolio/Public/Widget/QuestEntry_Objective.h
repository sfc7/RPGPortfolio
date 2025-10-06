// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "GameMode/GameManager/QuestManager.h"
#include "QuestEntry_Objective.generated.h"

class UCheckBox;
class UTextBlock;
struct FObjectiveDetail;
/**
 * UQuestEntry_Objective
 * 
 * 퀘스트의 현재 오브젝트 단계를 보여줄 UI
 */
UCLASS()
class RPGPORTFOLIO_API UQuestEntry_Objective : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetObjectiveData(FObjectiveDetail ObjectiveDataToSet, ARPGQuestSystemActor* CurrentQuestActorToSet);
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	// 완료 됐는지 확인하는 체크박스
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UCheckBox* IsCompleteCheckBox;

	// 현재 퀘스트 오브젝트 단계의 설명
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* Description;

	// 현재 퀘스트 오브젝트 단계의 정보
	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FObjectiveDetail ObjectiveData;

	// 현재 퀘스트
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI")
	ARPGQuestSystemActor* CurrentQuestActor;
};
