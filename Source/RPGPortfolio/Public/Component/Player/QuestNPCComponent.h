// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestNPCComponent.generated.h"

struct FDataTableRowHandle;
class UUserWidget;

/**
 * UQuestNPCComponent
 * 
 * NPC용 퀘스트 컴포넌트, 퀘스트 UI, 상호작용, 대화시스템 등등
 */
UCLASS()
class RPGPORTFOLIO_API UQuestNPCComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UQuestNPCComponent();

	//@ 퀘스트 수락 UI 표시
	UFUNCTION(BlueprintCallable)
	void DisplayQuest();

	//@ 퀘스트 보상 UI 표시
	UFUNCTION(BlueprintCallable)
	void DisplayRewards();

	//@ 상호작용 처리, 퀘스트 상태에 따른 분기 처리할 예정
	UFUNCTION(BlueprintCallable)
	FString InteractWith();

	//@ 대화 UI 표시
	UFUNCTION(BlueprintCallable)
	void DisplayConversation();
	
protected:
	virtual void BeginPlay() override;
	
private:
	//@ 이 NPC가 관리하는 퀘스트 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category = "Quest")
	FDataTableRowHandle QuestData;

	//@ 소유자 오브젝티브 ID(퀘스트데이터에서 쓰이는 ID)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "Quest")
	FString OwnerObjectiveID;

	//@ 대화 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "Conversation")
	TSubclassOf<UUserWidget> ConversationWidgetClass;
};
