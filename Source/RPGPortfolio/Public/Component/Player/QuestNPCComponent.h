// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestNPCComponent.generated.h"

struct FDataTableRowHandle;
UCLASS()
class RPGPORTFOLIO_API UQuestNPCComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UQuestNPCComponent();

	UFUNCTION(BlueprintCallable)
	void DisplayQuest();

	UFUNCTION(BlueprintCallable)
	FString InteractWith();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void LookAt();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category = "Quest")
	FDataTableRowHandle QuestData;

};
