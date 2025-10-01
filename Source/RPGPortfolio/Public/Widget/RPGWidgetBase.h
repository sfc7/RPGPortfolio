// LJS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGWidgetBase.generated.h"

class UPlayerUIComponent;
class UMonsterUIComponent;
/**
 * URPGWidgetBase
 * 
 * 모든 위젯의 Base 클래스
 */
UCLASS()
class RPGPORTFOLIO_API URPGWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	//@ 몬스터 생성 위젯 초기화 (몬스터 UI 컴포넌트 설정)
	UFUNCTION(BlueprintCallable)
	void InitMonsterCreatedWidget(AActor* OwningMonster);

protected:
	virtual void NativeOnInitialized() override;

	//@ 플레이어 UI 컴포넌트 초기화 이벤트
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PlayerUIComponentInitialized"))
	void BP_PlayerUIComponentInitialized(UPlayerUIComponent* PlayerUIComponent);

	//@ 몬스터  UI 컴포넌트 초기화 이벤트
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MonsterUIComponentInitialized"))
	void BP_MonsterUIComponentInitialized(UMonsterUIComponent* MonsterUIComponent);
};
