// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "PlayerUIComponent.generated.h"


/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerUIComponent : public UUIComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentMpChanged;
};
