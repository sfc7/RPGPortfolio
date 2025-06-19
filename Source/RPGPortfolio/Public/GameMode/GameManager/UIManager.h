// LJS

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UDataAsset_RPGUIData;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UUIManager();

	UFUNCTION(BlueprintCallable)
	void ShowUIAsync(const EUICategory& UICategory, UWorld* World);
	
	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetUIWidgetClass(const EUICategory& UICategory) const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	UPROPERTY()
	UDataAsset_RPGUIData* RPGUIData;
};
