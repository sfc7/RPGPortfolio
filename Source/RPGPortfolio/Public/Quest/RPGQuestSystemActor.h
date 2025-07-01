// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameMode/GameManager/QuestManager.h"
#include "RPGQuestSystemActor.generated.h"



UCLASS()
class RPGPORTFOLIO_API ARPGQuestSystemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGQuestSystemActor();

	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDtoSet);
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuest QuestDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStageDetail CurrentStageDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> CurrentObjectiveDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsComplete;
};
