// LJS


#include "Widget/RPGWidgetBase.h"
#include "Component/Player/PlayerUIComponent.h"
#include "Component/Monster/MonsterUIComponent.h"
#include "Interface/UIInterface.h"

void URPGWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const IUIInterface* UIInterface = Cast<IUIInterface>(GetOwningPlayerPawn());
	if (UIInterface) return;

	const UPlayerUIComponent* PlayerUIComponent = UIInterface->GetPlayerUIComponent();
	if (!IsValid(PlayerUIComponent)) return;

	BP_PlayerUIComponentInitialized(const_cast<UPlayerUIComponent*>(PlayerUIComponent));	
}

void URPGWidgetBase::InitMonsterCreatedWidget(AActor* OwningMonster)
{
	const IUIInterface* UIInterface = Cast<IUIInterface>(OwningMonster);
	if (UIInterface) return;
	
	const UMonsterUIComponent* MonsterUIComponent = UIInterface->GetMonsterUIComponent();
	if (!IsValid(MonsterUIComponent)) return;
	
	BP_MonsterUIComponentInitialized(const_cast<UMonsterUIComponent*>(MonsterUIComponent));
}


