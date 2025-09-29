// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/UIInterface.h"
#include "Interface/UInteractionInterface.h"
#include "RPGCharacterBase.generated.h"


class URPGAttributeSet;
class URPGAbilitySystemComponent;
class UDataAsset_AbilitySetBase;
class UCombatComponentBase;
class UMotionWarpingComponent;

/**
 * ARPGCharacterBase
 * 
 * 모든 ACharacter의 Base 클래스 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGCharacterBase : public ACharacter, public IAbilitySystemInterface, public IUIInterface, public IUInteractionInterface
{
	GENERATED_BODY()

public:
	ARPGCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	FORCEINLINE URPGAbilitySystemComponent* GetRPGAbilitySystemComponent() const {return RPGAbilitySystemComponent;}
	
	FORCEINLINE URPGAttributeSet* GetRPGAttributeSet() const {return RPGAttributeSet;}

	UFUNCTION(BlueprintCallable)
	virtual UCombatComponentBase* GetCombatComponent() const;

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UDataAsset_AbilitySetBase> GetCharacterStartUpData() const { return CharacterStartUpData; } 
	
	virtual void CreateDefaultAttributeSet();

	virtual UUIComponentBase* GetUIComponent() const override;

	virtual UMotionWarpingComponent* GetMotionWarpingComponent() const {return MotionWarpingComponent;}

	UFUNCTION(BlueprintCallable)
	void SetInteractTargetActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	AActor* GetInteractTargetActor() const {return InteractTargetActor;}
	
	UFUNCTION(BlueprintCallable)
	FString GetObjectiveName();

	// 상호작용 및 퀘스트에 사용할 오브젝트 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveName;
protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	URPGAbilitySystemComponent* RPGAbilitySystemComponent;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	URPGAttributeSet* RPGAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;

	//@ GAS에 사용할 GameplayAbility DataAsset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_AbilitySetBase> CharacterStartUpData;

	//@ 상호작용 대상 캐싱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InteractTargetActor;
};
