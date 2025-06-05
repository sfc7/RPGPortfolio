// LJS


#include "GameAbilitySystem/GameplayTask/RPGAT_ExecuteTaskOnTick.h"

URPGAT_ExecuteTaskOnTick::URPGAT_ExecuteTaskOnTick()
{
	bTickingTask = true;
}

void URPGAT_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

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
	URPGAT_ExecuteTaskOnTick* Node = NewAbilityTask<URPGAT_ExecuteTaskOnTick>(OwningAbility);

	return Node;
}