// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGCharacterBase.generated.h"

class URPGAttributeSet;
class URPGAbilitySystemComponent;
class UDataAsset_AbilitySetBase;
class UCombatComponentBase;

UCLASS()
class RPGPORTFOLIO_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARPGCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	FORCEINLINE URPGAbilitySystemComponent* GetRPGAbilitySystemComponent() const {return RPGAbilitySystemComponent;}
	
	FORCEINLINE URPGAttributeSet* GetRPGAttributeSet() const {return RPGAttributeSet;}

	UFUNCTION(BlueprintCallable)
	virtual UCombatComponentBase* GetCombatComponent() const;
	
	virtual void CreateDefaultAttributeSet();

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	URPGAbilitySystemComponent* RPGAbilitySystemComponent;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	URPGAttributeSet* RPGAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterStartUpData;
};
