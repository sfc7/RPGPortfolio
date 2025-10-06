// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/QuestWidget.h"
#include "QuestRewardWidget.generated.h"

/**
 * UQuestRewardWidget
 * 
 * 퀘스트 보상 수령시 나오는 UI 
 */
UCLASS()
class RPGPORTFOLIO_API UQuestRewardWidget : public UQuestWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	virtual void OnAcceptButtonClicked() override;
};
