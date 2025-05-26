// LJS


#include "Widget/RPGWidgetBase.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Monster/MonsterUIComponent.h"
#include "Interface/UIInterface.h"

void URPGWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IUIInterface* UIInterface = Cast<IUIInterface>(GetOwningPlayerPawn());
	
	if (UIInterface)
	{
		UPlayerUIComponent* PlayerUIComponent = UIInterface->GetPlayerUIComponent();
		if (PlayerUIComponent)
		{
			BP_PlayerUIComponentInitialized(PlayerUIComponent);
		}
	}
}

void URPGWidgetBase::InitMonsterCreatedWidget(AActor* OwningMonster)
{
	IUIInterface* UIInterface = Cast<IUIInterface>(OwningMonster);

	if (UIInterface)
	{
		UMonsterUIComponent* MonsterUIComponent = UIInterface->GetMonsterUIComponent();
		if (MonsterUIComponent)
		{
			BP_MonsterUIComponentInitialized(MonsterUIComponent);
		}
	}
		
}


