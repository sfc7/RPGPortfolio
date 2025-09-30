// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIComponentBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

/**
 * UUIComponentBase
 * 
 * UI에 사용할 컴포넌트의 Base
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UUIComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Hp 변동에 따른 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHpChanged;

	//@ 컴포넌트 오너용 템플릿 함수
#pragma region GetOwningPawn(), GetOwningController()
	template<class T>
	T* GetOwningPawn() const
	{
		return CastChecked<T>(GetOwner());
	}
	
	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		return GetOwningPawn<APawn>()->GetController<T>();
	}
#pragma endregion
	
		
};
