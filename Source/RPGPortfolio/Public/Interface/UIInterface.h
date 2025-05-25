// LJS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIInterface.generated.h"

class UUIComponentBase;
class UPlayerUIComponent;
class UMonsterUIComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPORTFOLIO_API IUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UUIComponentBase* GetUIComponent() const = 0;

	virtual UPlayerUIComponent* GetPlayerUIComponent() const;

	virtual UMonsterUIComponent* GetMonsterUIComponent() const;
};
