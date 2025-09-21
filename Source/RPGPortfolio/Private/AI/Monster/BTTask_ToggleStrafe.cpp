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

	URPGAbilitySystemComponent* ASC = MonsterCharacter->GetRPGAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	// Strafe 활성화
	if (ShouldEnable)
	{
		MonsterCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		if (ShouldChangeMaxWalkSpeed)
		{
			MonsterCharacter->GetCharacterMovement()->MaxWalkSpeed = StrafeWalkSpeed;
		}
		
		if (!ASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_Strafe))
		{
			ASC->AddLooseGameplayTag(RPGGameplayTag::Monster_Status_Strafe);	
		}
	}
	// Strafe 비 활성화
	else
	{
		MonsterCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		// MaxWalkSpeed는 블랙보드에 캐싱되어 있어서 그걸 가져옴
		const float BlackboardMaxSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MaxWalkSpeedKey.SelectedKeyName);
		if (MonsterCharacter->GetCharacterMovement()->MaxWalkSpeed != BlackboardMaxSpeed)
		{
			MonsterCharacter->GetCharacterMovement()->MaxWalkSpeed = BlackboardMaxSpeed;
		}
		
		if (ASC->HasMatchingGameplayTag(RPGGameplayTag::Monster_Status_Strafe))
		{
			ASC->RemoveLooseGameplayTag(RPGGameplayTag::Monster_Status_Strafe);
		}
	}
}
