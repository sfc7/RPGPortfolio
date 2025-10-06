// LJS


#include "GameAbilitySystem/GameplayTask/RPGAT_ExecuteTaskOnTick.h"

URPGAT_ExecuteTaskOnTick::URPGAT_ExecuteTaskOnTick()
{
	bTickingTask = true;
}

void URPGAT_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	// 델리게이트 브로드캐스트 가능 여부 확인 (AbilityTask 자체 지원 함수)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}

URPGAT_ExecuteTaskOnTick* URPGAT_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	// Tick 태스크 팩토리 생성 과정
	URPGAT_ExecuteTaskOnTick* Node = NewAbilityTask<URPGAT_ExecuteTaskOnTick>(OwningAbility);

	return Node;
}