// LJS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGWidgetBase.generated.h"

class UPlayerUIComponent;
class UMonsterUIComponent;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void InitMonsterCreatedWidget(AActor* OwningMonster);

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PlayerUIComponentInitialized"))
	void BP_PlayerUIComponentInitialized(UPlayerUIComponent* PlayerUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MonsterUIComponentInitialized"))
	void BP_MonsterUIComponentInitialized(UMonsterUIComponent* MonsterUIComponent);

	
};
