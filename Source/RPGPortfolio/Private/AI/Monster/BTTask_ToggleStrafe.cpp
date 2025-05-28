// LJS


#include "AI/Monster/BTTask_ToggleStrafe.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MonsterCharacter.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_ToggleStrafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_ToggleStrafe::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (MonsterCharacter)
	{
		ASC = MonsterCharacter->GetRPGAbilitySystemComponent();
	}
	
	if (ShouldEnable)
	{
		MonsterCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		if (ShouldChangeMaxWalkSpeed)
		{
			MonsterCharacter->GetCharacterMovement()->MaxWalkSpeed = StrafeWalkSpeed;
		}
		
		if (ASC)
		{
			if (!ASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_Strafe))
			{
				ASC->AddLooseGameplayTag(RPGGameplayTag::Monster_Status_Strafe);	
			}
		}
	}
	else
	{
		MonsterCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		float BlackboardMaxSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MaxWalkSpeedKey.SelectedKeyName);
		if (MonsterCharacter->GetCharacterMovement()->MaxWalkSpeed != BlackboardMaxSpeed)
		{
			MonsterCharacter->GetCharacterMovement()->MaxWalkSpeed = BlackboardMaxSpeed;
		}

		if (ASC)
		{
			if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_Strafe))
			{
				ASC->RemoveLooseGameplayTag(RPGGameplayTag::Monster_Status_Strafe);
			}
		}
	}
}
