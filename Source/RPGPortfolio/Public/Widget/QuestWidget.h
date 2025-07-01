// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/RPGWidgetBase.h"
#include "GameMode/GameManager/QuestManager.h"
#include "QuestWidget.generated.h"


class UButton;
class UTextBlock;
struct FQuest;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UQuestWidget : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FQuest GetQuest() const { return QuestDetails; }

	UFUNCTION(BlueprintCallable)
	void SetQuest(FQuest QuestDetailsToSet);

	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDToSet);
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnAcceptButtonClicked();
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestDescription;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* StageDescription;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UButton* AcceptButton;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))	
	UButton* DeclineButton;

	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FQuest QuestDetails;

	UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (BindWidget))
	FName QuestID;
};
