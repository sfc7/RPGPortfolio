// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestNPCComponent.generated.h"

struct FDataTableRowHandle;
class UUserWidget;
UCLASS()
class RPGPORTFOLIO_API UQuestNPCComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UQuestNPCComponent();

	UFUNCTION(BlueprintCallable)
	void DisplayQuest();

	UFUNCTION(BlueprintCallable)
	void DisplayRewards();

	UFUNCTION(BlueprintCallable)
	FString InteractWith();

	UFUNCTION(BlueprintCallable)
	void DisplayConversation();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void LookAt();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category = "Quest")
	FDataTableRowHandle QuestData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "Quest")
	FString OwnerObjectiveID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "Conversation")
	TSubclassOf<UUserWidget> ConversationWidgetClass;
};
