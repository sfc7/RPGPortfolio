// LJS

#pragma once

#include "CoreMinimal.h"
#include "Widget/QuestWidget.h"
#include "QuestRewardWidget.generated.h"

/**
 * 
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
