// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "MonsterUIComponent.generated.h"

class URPGWidgetBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UMonsterUIComponent : public UUIComponentBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void RegisterMonsterDrawnWidget(URPGWidgetBase* WidgetToRegister);

	UFUNCTION(BlueprintCallable)
		void RemoveMonsterDrawnWidget();
private:
	TArray<URPGWidgetBase*> MonsterDrawnWidgetArray;
};
