// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGCharacterBase.generated.h"

class UMainAttributeSet;
class UMainAbilitySystemComponent;
class UDataAsset_AbilitySetBase;

UCLASS()
class RPGPORTFOLIO_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARPGCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	FORCEINLINE UMainAbilitySystemComponent* GetMainAbilitySystemComponent() const {return MainAbilitySystemComponent;}

	FORCEINLINE UMainAttributeSet* GetMainAttributeSet() const {return MainAttributeSet;}

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UMainAbilitySystemComponent* MainAbilitySystemComponent;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UMainAttributeSet* MainAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterStartUpData;
};
