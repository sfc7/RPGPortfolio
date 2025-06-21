// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_RPGUIData.generated.h"

UENUM(BlueprintType)
enum class EUICategory : uint8
{
	GameOverUI,
	VictoryUI,
	PauseMenuUI,
	None,
};

USTRUCT(BlueprintType)
struct FUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EUICategory UIType = EUICategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> UIWidgetClass;
};
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_RPGUIData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDataAsset_RPGUIData(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetUIWidgetClass(const EUICategory& UICategory) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData VictoryUI;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData GameOverUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FUIData PauseMenuUI;
};
