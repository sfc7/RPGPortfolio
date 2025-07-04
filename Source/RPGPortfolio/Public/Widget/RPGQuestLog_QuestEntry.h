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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestSelected, FName, QuestID, ARPGQuestSystemActor*, QuestActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestTracked, ARPGQuestSystemActor*, QuestActor, bool, IsCheck);
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGQuestLog_QuestEntry : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	FOnQuestSelected OnQuestSelected;

	FOnQuestTracked OnQuestTracked;

	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDToSet);

	UFUNCTION(BlueprintCallable)
	void SetQuestActor(ARPGQuestSystemActor* QuestActorToSet);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnTrackCheckBoxClicked(bool IsCheck);

	UFUNCTION()
	void OnAcceptButtonClicked();

	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UButton* SelectButton;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UCheckBox* TrackCheckBox;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FName QuestID;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuestSystemActor* QuestActor;
};
