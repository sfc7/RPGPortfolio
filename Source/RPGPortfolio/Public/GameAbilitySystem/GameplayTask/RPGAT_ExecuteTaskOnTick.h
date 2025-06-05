// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RPGAT_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate,float,DeltaTime);

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API URPGAT_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()
public:
	URPGAT_ExecuteTaskOnTick();

	virtual void TickTask(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static URPGAT_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};
