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
 * 
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

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UCheckBox* IsCompleteCheckBox;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FObjectiveDetail ObjectiveData;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI")
	ARPGQuestSystemActor* CurrentQuestActor;
};
