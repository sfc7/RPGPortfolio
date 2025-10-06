// LJS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RPGAT_ExecuteTaskOnTick.generated.h"

//@ AbilityTask Tick 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate,float,DeltaTime);

/**
 * URPGAT_ExecuteTaskOnTick
 * 
 * AbilityTask를 Tick으로 사용하기 위한 클래스, 매 프레임 마다 델리게이트 브로드캐스트 하고 종료기능 포함
 */
UCLASS()
class RPGPORTFOLIO_API URPGAT_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()
public:
	URPGAT_ExecuteTaskOnTick();

	virtual void TickTask(float DeltaTime) override;

	//@ Tick 태스크 생성 팩토리 함수
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static URPGAT_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	//@ AbilityTask Tick 델리게이트 
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};
