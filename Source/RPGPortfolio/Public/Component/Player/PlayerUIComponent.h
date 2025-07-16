// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitPlayerUIbyClassDelegate, EPlayerCharacterClass , PlayerCharacterClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSkillCooldownBeginDelegate, int32, Index, FGameplayTag, SkillAbilityTag, float, TotalCooldownTime, float, RemaningCooldownTime);
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerUIComponent : public UUIComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentMpChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnInitPlayerUIbyClassDelegate OnInitPlayerUIbyClassDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSkillCooldownBeginDelegate OnSkillCooldownBeginDelegate;	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameOver", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UUserWidget> GameOverUIWidgetClass;
};
	