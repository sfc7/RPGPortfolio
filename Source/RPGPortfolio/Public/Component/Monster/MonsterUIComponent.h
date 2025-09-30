// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "MonsterUIComponent.generated.h"

class URPGWidgetBase;
/**
 * UMonsterUIComponent
 * 
 * Monster의 UI에 사용할 컴포넌트
 */
UCLASS()
class RPGPORTFOLIO_API UMonsterUIComponent : public UUIComponentBase
{
	GENERATED_BODY()
public:
	//@ BossMonster용 HUD 위젯 등록
	UFUNCTION(BlueprintCallable)
		void RegisterMonsterDrawnWidget(URPGWidgetBase* WidgetToRegister);

	UFUNCTION(BlueprintCallable)
		void RemoveMonsterDrawnWidget();
private:
	//@ BossMonster용 HUD 위젯 Array
	TArray<URPGWidgetBase*> MonsterDrawnWidgetArray;
};
