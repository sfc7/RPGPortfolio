// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAsset/Item/DataAsset_RPGItemData.h"
#include "QuestManager.generated.h"

class ARPGQuestSystemActor;
class UDataAsset_RPGItemData;

//@ 퀘스트 목적 타입 열거형
UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	Location,
	Kill,
	Conversation,
	Collect
};

//@ 퀘스트 세부사항 구조체
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

//@ 퀘스트 스테이지(퀘스트 내부 단위) 세부사항 구조체
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
	TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32> ItemRewardAndQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoldReward;
};

//@ 퀘스트 구조체
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

//@ 퀘스트 저장 데이터 구조체
USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, int32> QuestProgress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentStage;
	
};

//@ 퀘스트 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, ARPGQuestSystemActor*, QuestActor);
/**
 * UQuestManager
 * 
 * 퀘스트, 퀘스트 액터 관리 서브시스템
 */
UCLASS()
class RPGPORTFOLIO_API UQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//@ 액터단위로 새 퀘스트 추가
	UFUNCTION(BlueprintCallable)
	ARPGQuestSystemActor* AddNewQuest(FName QuestID);

	//@ 퀘스트 완료
	UFUNCTION(BlueprintCallable)
	void CompleteQuest(FName QuestID);

	//@ 활성화된 퀘스트 조회
	UFUNCTION(BlueprintPure)
	bool QueryActiveQuest(FName QuestID);

	//@ 퀘스트 추적
	UFUNCTION(BlueprintCallable)
	void TrackQuest();

	//@ 퀘스트 데이터 테이블 가져오기
	UFUNCTION(BlueprintCallable)
	UDataTable* GetQuestDataTable();

	//@ 데이터 테이블에서 퀘스트 가져오기
	UFUNCTION(BlueprintCallable)
	FQuest GetQuestFromDataTable(FName QuestID);

	//@ 현재 퀘스트들 가져오기
	UFUNCTION(BlueprintCallable)
	TArray<ARPGQuestSystemActor*> GetCurrentQuests();
	
	//@ 현재 진행중인 퀘스트들 가져오기
	UFUNCTION(BlueprintCallable)
	TArray<FName> GetCurrentActiveQuests();

	//@ 완료된 퀘스트들 가져오기
	UFUNCTION(BlueprintCallable)
	TArray<FName> GetCompletedQuests();

	//@ 활성 퀘스트 설정
	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveQuests(TArray<FName> CurrentActiveQuestsToSet);

	//@ 완료된 퀘스트 설정
	UFUNCTION(BlueprintCallable)
	void SetCompleteActiveQuests(TArray<FName> CompletedQuestsToSet);

	//@ 모든 퀘스트 초기화
	UFUNCTION(BlueprintCallable)
	void ClearAllQuests();

	//@ 퀘스트 완료 상태로 변환
	UFUNCTION(BlueprintCallable)
	void TurnInQuest(FName QuestID);

	//@ 퀘스트 액터 가져오기
	UFUNCTION(BlueprintCallable)
	ARPGQuestSystemActor* GetQuestActor(FName QuestID); 

	//@ 퀘스트 보상 지급
	UFUNCTION(BlueprintCallable)
	bool GrantQuestRewards(FName QuestID, APlayerCharacterBase* Player);

	//@ 아이템 보상 지급 
	UFUNCTION(BlueprintCallable)
	bool GrantItemRewards(const TMap<TSoftObjectPtr<UDataAsset_RPGItemData>, int32>& ItemRewards, UInventoryComponent* PlayerInventory);

	//@ 골드 보상 지급
	UFUNCTION(BlueprintCallable)
	void GrantGoldReward(int32 GoldAmount, UInventoryComponent* PlayerInventory);

	//@ 퀘스트 완료 델리게이트
	FOnQuestCompleted OnQuestCompleted;
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	//@ 현재 활성 퀘스트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> CurrentActiveQuests;

	//@ 완료된 퀘스트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> CompletedQuests;

	//@ 현재 추적 중인 퀘스트(UI에서 트래킹 체크시 사용함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName CurrentTrackingQuests;

	//@ 현재 퀘스트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ARPGQuestSystemActor*> CurrentQuests;

	//@ 퀘스트 데이터 정보 캐싱 테이블
	UPROPERTY(BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	UDataTable* QuestDataTable;
};
