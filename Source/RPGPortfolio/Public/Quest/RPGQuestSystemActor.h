// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameMode/GameManager/QuestManager.h"
#include "RPGQuestSystemActor.generated.h"

//@ 퀘스트 오브젝트 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveHeard);

/**
 * ARPGQuestSystemActor
 * 
 * 퀘스트 시스템용 액터, 현재 퀘스트 시스템은 액터 단위로 구현함
 * 퀘스트 - 퀘스트 오브젝트 - 퀘스트 스테이지의 3단 구조로 설계
 */
UCLASS()
class RPGPORTFOLIO_API ARPGQuestSystemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGQuestSystemActor();

	//@ 퀘스트 ID 가져오기
	UFUNCTION(BlueprintCallable)
	FName GetQuestID() const { return QuestID;}

	//@ 퀘스트 현재 단계 가져오기
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentStage() const { return CurrentStage; }

	//@ 완료 여부 가져오기
	UFUNCTION(BlueprintCallable)
	bool GetIsComplete() const { return IsComplete; }

	//@ 퀘스트 상세 정보 가져오기
	UFUNCTION(BlueprintCallable)
	FQuest GetQuestDetailVariable() const { return QuestDetails; }

	//@ 퀘스트 ID 설정
	UFUNCTION(BlueprintCallable)
	void SetQuestID(FName QuestIDtoSet);

	//@ 퀘스트 단계 설정
	UFUNCTION(BlueprintCallable)
	void SetCurrentStage(int32 QuestStageToSet);

	//@ 상호 작용시 퀘스트 오브젝트 ID 관련 처리
	UFUNCTION(BlueprintCallable)
	void OnObjectiveIDHeard(FString ObjectiveID);
	
	//@ 퀘스트 상세 정보 로드
	UFUNCTION(BlueprintCallable)
	void GetQuestDetails();

	//@ 퀘스트 오브젝트 ID로 퀘스트 오브젝트 데이터 가져오기
	UFUNCTION(BlueprintPure)
	FObjectiveDetail GetObjectiveDataByID(FString ObjectiveID);

	UFUNCTION(BlueprintCallable)
	TMap<FString, int32> GetCurrentObjectiveProgress() { return CurrentObjectiveProgress; }

	//@ 현재 퀘스트 오브젝트 진행도 설정
	UFUNCTION(BlueprintCallable)
	void SetCurrentObjectiveProgress(TMap<FString, int32> ObjectiveProgressToSet);

	//@ 현재 퀘스트 오브젝트 완료
	UFUNCTION(BlueprintCallable)
	void IsObjectiveComplete(FString ObjectiveID);

	//@ 모든 목표 퀘스트 오브젝트 완료 확인
	UFUNCTION(BlueprintCallable)
	bool AreObjectivesComplete(); 

	//@ 퀘스트 오브젝트 완료 이벤트 
	FOnObjectiveHeard OnObjectiveHeard;
protected:
	virtual void BeginPlay() override;

	//@ 퀘스트 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	//@ 퀘스트 상세 정보(구조체)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuest QuestDetails;

	//@ 현재 퀘스트 Stage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStage = 0;

	//@ 현재 퀘스트 Stage의 상세 정보(구조체)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStageDetail CurrentStageDetails;

	//@ 현재 퀘스트 오브젝트 진행도 맵 (목표ID -> 진행도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> CurrentObjectiveProgress;

	//@ 퀘스트 완료 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsComplete;
};
