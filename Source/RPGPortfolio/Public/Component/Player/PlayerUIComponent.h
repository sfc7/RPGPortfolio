// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitPlayerUIbyClassDelegate, EPlayerCharacterClass , PlayerCharacterClass);
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

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnInitPlayerUIbyClassDelegate OnInitPlayerUIbyClassDelegate;

};
