// LJS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIInterface.generated.h"

class UUIComponentBase;
class UPlayerUIComponent;
class UMonsterUIComponent;

//@ UI 인터페이스 UObject 래퍼 클래스 - 언리얼 엔진 인터페이스 시스템용
UINTERFACE(MinimalAPI)
class UUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
* IUIInterface
* 
* UI 컴포넌트 접근 인터페이스
*/
class RPGPORTFOLIO_API IUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//@ 기본 UI 컴포넌트 가져오기, 순수가상함수로 
	virtual UUIComponentBase* GetUIComponent() const = 0;

	//@ 플레이어 UI 컴포넌트 가져오기 
	virtual UPlayerUIComponent* GetPlayerUIComponent() const;

	//@ 몬스터 UI 컴포넌트 가져오기
	virtual UMonsterUIComponent* GetMonsterUIComponent() const;
};
