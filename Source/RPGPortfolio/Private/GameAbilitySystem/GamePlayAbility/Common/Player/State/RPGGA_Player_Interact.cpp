// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_Interact.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameMode/GameManager/InteractManager.h"
#include "Character/Player/PlayerCharacterBase.h"

URPGGA_Player_Interact::URPGGA_Player_Interact()
{
}

void URPGGA_Player_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 상호작용 매니저에서 가져온 상호 작용 함수 실행
	if (AActor* TargetActor = GetPlayerCharacterFromActorInfo()->GetInteractTargetActor())
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UInteractManager>()->StartInteract(TargetActor, GetPlayerCharacterFromActorInfo(), EInteractType::NPC);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void URPGGA_Player_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool URPGGA_Player_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	// 상호작용 이전에 가능한 상태인지 GameplayTag로 체크
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		return ASC->HasMatchingGameplayTag(RPGGameplayTag::Player_Status_CanInteract);
	}
	
	return false;
}