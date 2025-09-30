// LJS

#pragma once

#include "CoreMinimal.h"
#include "Component/UIComponentBase.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitPlayerUIbyClassDelegate, EPlayerCharacterClass , PlayerCharacterClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSkillCooldownBeginDelegate, int32, Index, FGameplayTag, SkillAbilityTag, float, TotalCooldownTime, float, RemaningCooldownTime);
/**
 * UPlayerUIComponent
 * 
 * PlayerCharacter의 UI에 사용할 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayerUIComponent : public UUIComponentBase
{
	GENERATED_BODY()

public:
	//@ Mp 변동에 따른 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentMpChanged;

	//@ PlayerUI 초기화시 델리게이트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnInitPlayerUIbyClassDelegate OnInitPlayerUIbyClassDelegate;

	//@ 스킬 쿨다운 UI관련 델리게이트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSkillCooldownBeginDelegate OnSkillCooldownBeginDelegate;	
	
private:
	//@ GameOver UI 소프트레퍼런스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameOver", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UUserWidget> GameOverUIWidgetClass;
};
	