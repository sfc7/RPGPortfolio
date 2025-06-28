// LJS

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGNPCCharacterBase.generated.h"

class APlayerCharacterBase;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGNPCCharacterBase : public ARPGCharacterBase
{
	GENERATED_BODY()
public:
	ARPGNPCCharacterBase();

	APlayerCharacterBase* AccessPlayerCharacter;

	UFUNCTION()
	void SetAccessPlayerCharacter(APlayerCharacterBase* PlayerCharacter) { AccessPlayerCharacter = PlayerCharacter; }
};
