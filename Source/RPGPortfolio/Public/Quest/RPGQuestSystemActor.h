// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameMode/GameManager/QuestManager.h"
#include "RPGQuestSystemActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveHeard);


UCLASS()
class RPGPORTFOLIO_API ARPGQuestSystemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGQuestSystemActor();

	UFUNCTION(BlueprintCallable)
	FName GetQuestID() const { return QuestID;}

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentStage() const { return CurrentStage; }

	UFUNCTION(BlueprintCallable)
	bool GetIsComplete() const { return IsComplete; }

	UFUNCTION(BlueprintCallable)
	FQuest GetQuestDetailVariable() const { return QuestDetails; }
	
	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDtoSet);

	UFUNCTION(BlueprintCallable)
	void SetCurrentStage(int32 QuestStageToSet);

	UFUNCTION(BlueprintCallable)
	void OnObjectiveIDHeard(FString ObjectiveID);

	UFUNCTION(BlueprintCallable)
	void GetQuestDetails();

	UFUNCTION(BlueprintPure)
	FObjectiveDetail GetObjectiveDataByID(FString ObjectiveID);

	UFUNCTION(BlueprintCallable)
	TMap<FString, int32> GetCurrentObjectiveProgress() { return CurrentObjectiveProgress; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentObjectiveProgress(TMap<FString, int32> ObjectiveProgressToSet);

	UFUNCTION(BlueprintCallable)
	void IsObjectiveComplete(FString ObjectiveID);

	UFUNCTION(BlueprintCallable)
	bool AreObjectivesComplete(); 

	FOnObjectiveHeard OnObjectiveHeard;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuest QuestDetails;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStageDetail CurrentStageDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> CurrentObjectiveProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsComplete;
};
