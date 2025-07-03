// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestManager.generated.h"

class ARPGQuestSystemActor;
class UDataAsset_RPGItemData;

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	Location,
	Kill,
	Conversation,
	Collect
};


USTRUCT(BlueprintType)
struct FObjectiveDetail
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveType ObjectiveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOptional;
};

USTRUCT(BlueprintType)
struct FStageDetail
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StageDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectiveDetail> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UDataAsset_RPGItemData*, int32> ItemRewardAndQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoldReward;
};

USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TrackingDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStageDetail> QuestStages;
};

USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, int32> QuestProgress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentStage;
	
};
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	ARPGQuestSystemActor* AddNewQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	void CompleteQuest(FName QuestID);

	UFUNCTION(BlueprintPure)
	bool QueryActiveQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	void TrackQuest();

	UFUNCTION(BlueprintCallable)
	UDataTable* GetQuestDataTable();

	UFUNCTION(BlueprintCallable)
	FQuest GetQuestFromDataTable(FName QuestID);

	UFUNCTION(BlueprintCallable)
	TArray<ARPGQuestSystemActor*> GetCurrentQuests();

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetCurrentActiveQuests();

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetCompletedQuests();

	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet);

	UFUNCTION(BlueprintCallable)
	void SetCompleteActiveQuests(TArray<FName> CompletedQuestsToSet);

	UFUNCTION(BlueprintCallable)
	void ClearAllQuests();

	UFUNCTION(BlueprintCallable)
	void TurnInQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	ARPGQuestSystemActor* GetQuestActor(FName QuestID); 

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> CurrentActiveQuests;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> CompletedQuests;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName CurrentTrackingQuests;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ARPGQuestSystemActor*> CurrentQuests;

	UPROPERTY(BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	UDataTable* QuestDataTable;
};
