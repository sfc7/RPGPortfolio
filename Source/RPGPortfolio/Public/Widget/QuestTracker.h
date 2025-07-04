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
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UQuestTracker : public URPGWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(BlueprintReadWrite, Category = "QuestUI", meta = (BindWidget))
	UVerticalBox* ObjectiveBox;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuestSystemActor* QuestActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestUI")
	TSubclassOf<UUserWidget> QuestEntry_ObjectiveWidgetClass;

	void UpdateQuestActor(ARPGQuestSystemActor* QuestActorToSet);

	void SetQuestActor(ARPGQuestSystemActor* QuestActorToSet);

	UFUNCTION()
	void QuestCompleted(ARPGQuestSystemActor* QuestActorToSet);

	UFUNCTION()
	void OnObjectiveHeard();
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;
};
