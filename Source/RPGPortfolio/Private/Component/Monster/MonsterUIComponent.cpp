// LJS


#include "Component/Monster/MonsterUIComponent.h"

#include "Widget/RPGWidgetBase.h"

void UMonsterUIComponent::RegisterMonsterDrawnWidget(URPGWidgetBase* WidgetToRegister)
{
	MonsterDrawnWidgetArray.Add(WidgetToRegister);
}

void UMonsterUIComponent::RemoveMonsterDrawnWidget()
{
	if (MonsterDrawnWidgetArray.IsEmpty()) return;
	
	for (URPGWidgetBase* DrawnWidget : MonsterDrawnWidgetArray)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	MonsterDrawnWidgetArray.Empty();
}
