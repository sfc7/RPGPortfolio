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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UQuestLogWidget : public URPGWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnQuestSelected(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet);

	UFUNCTION()
	void OnQuestTracked(ARPGQuestSystemActor* QuestActorToSet, bool IsCheck);
	
	UFUNCTION()
	void DisplayQuestToRightDetail(FName QuestIDToSet, ARPGQuestSystemActor* QuestActorToSet);

	UFUNCTION(BlueprintCallable)
	void CreateRewardItemSlots(FQuest QuestDetail);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemSlotMasterClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Quest_LogEntryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> LogEntry_ObjectiveWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Quest_TrackWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestTracker* TrackWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* StageDescription;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestDescription;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestGold;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UScrollBox* QuestScrollBox;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UItemSlotContainer* RewardItemContainer;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UWidgetSwitcher* RightQuestDetailWidgetSwitcher;	

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UVerticalBox* ObjectiveVerticalBox;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI")
	ARPGQuestSystemActor* CurrentQuestActor;
};
