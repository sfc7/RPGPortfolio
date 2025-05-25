// LJS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIComponentBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UUIComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHpChanged;

protected:
#pragma region GetOwningPawn(), GetOwningController()
protected:
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
