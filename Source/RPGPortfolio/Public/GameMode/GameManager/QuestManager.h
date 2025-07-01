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
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddNewQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	void CompleteQuest();

	UFUNCTION(BlueprintPure)
	bool QueryActiveQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	void TrackQuest();

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

	UPROPERTY()
	UDataTable* QuestData;
};
