// LJS


#include "Widget/QuestEntry_Objective.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Quest/RPGQuestSystemActor.h"


void UQuestEntry_Objective::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UQuestEntry_Objective::NativePreConstruct()
{
	Super::NativePreConstruct();
}


void UQuestEntry_Objective::SetObjectiveData(FObjectiveDetail ObjectiveDataToSet, ARPGQuestSystemActor* CurrentQuestActorToSet)
{
	if (IsValid(CurrentQuestActorToSet))
	{
		// 현재 퀘스트 오브젝트 단계에 맞는 오브젝트 설정
		ObjectiveData = ObjectiveDataToSet;
		CurrentQuestActor = CurrentQuestActorToSet;
	
		FText ObjectiveDescription = ObjectiveData.ObjectiveDescription;
		int32 CurrentQuantity = ObjectiveData.Quantity;
		TMap<FString, int32> CurrentObjectiveProgress = CurrentQuestActor->GetCurrentObjectiveProgress();

		//오브젝트 ID로 진행도를 통해 완료 여부를 체크하고 체크박스로 완료 여부 표시
		if (CurrentObjectiveProgress.Find(ObjectiveData.ObjectiveID))
		{
			CurrentQuantity = *CurrentObjectiveProgress.Find(ObjectiveData.ObjectiveID);
			if (CurrentQuantity >= ObjectiveData.Quantity)
			{
				IsCompleteCheckBox->SetCheckedState(ECheckBoxState::Checked);
			}
			else 
			{
				IsCompleteCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
			}
		}
		
		Description->SetText(FText::FromString(
		ObjectiveData.ObjectiveDescription.ToString() + 
		FString::Printf(TEXT(" (%d/%d)"), CurrentQuantity, ObjectiveData.Quantity)
		));
	}
}
