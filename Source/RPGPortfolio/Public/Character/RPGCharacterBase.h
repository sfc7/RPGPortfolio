// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/UIInterface.h"
#include "RPGCharacterBase.generated.h"

class URPGAttributeSet;
class URPGAbilitySystemComponent;
class UDataAsset_AbilitySetBase;
class UCombatComponentBase;
class UMotionWarpingComponent;

UCLASS()
class RPGPORTFOLIO_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface, public IUIInterface
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

	virtual UUIComponentBase* GetUIComponent() const override;

	virtual UMotionWarpingComponent* GetMotionWarpingComponent() const {return MotionWarpingComponent;};
	
protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	URPGAbilitySystemComponent* RPGAbilitySystemComponent;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	URPGAttributeSet* RPGAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterStartUpData;
	
	
};
