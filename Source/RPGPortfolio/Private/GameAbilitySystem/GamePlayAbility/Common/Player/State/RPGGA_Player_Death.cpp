// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_Death.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Controller/PlayerCharacterController.h"
#include "GameMode/GameManager/DungeonProgressManager.h"
#include "GameMode/GameManager/GeneralGameManager.h"
#include "GameMode/GameManager/UIManager.h"

URPGGA_Player_Death::URPGGA_Player_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void URPGGA_Player_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(DeathMontage)) return;

	// 사망 애니메이션 재생 태스크 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("Player_Death"), DeathMontage, 1.0f,  NAME_None,
	true, 1.0f, 0.f,false);

	// 사망 애니메이션 완료 이벤트 바인딩
	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Player_Death::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Player_Death::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();
}

void URPGGA_Player_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 애니메이션 일시정지
	GetOwningComponentFromActorInfo()->bPauseAnims = true;
	// 캡슐 충돌 비활성화
	GetPlayerCharacterFromActorInfo()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	APlayerCharacterController* PC = GetPlayerControllerFromActorInfo();
	if (IsValid(PC))
	{
		FInputModeUIOnly InputModeUI;
		PC->SetInputMode(InputModeUI);
		PC->SetShowMouseCursor(true);
	}

	// 던전 진행 매니저에서 게임 오버 상태 설정
	GetWorld()->GetGameInstance()->GetSubsystem<UGeneralGameManager>()->GetDungeonProgressManager()->SetDungeonState(EDungeonState::GameOver);
}

void URPGGA_Player_Death::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
