// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Monster/RPGGA_Monster_Attack_EarthQuake.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MeshPaintVisualize.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/MonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

URPGGA_Monster_Attack_EarthQuake::URPGGA_Monster_Attack_EarthQuake()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGGA_Monster_Attack_EarthQuake::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,TEXT("Player_Attack_EarthQuake"), AttackMontage, 1.0f,  NAME_None,
true, 1.0f, false);

	PlayMontageTask->OnCompleted.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RPGGameplayTag::Character_Event_AttackHit_AreaAttack, nullptr, false, true
	);

	WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::CheckBoxTrace);
	WaitGameplayEvent->ReadyForActivation();

	
}

void URPGGA_Monster_Attack_EarthQuake::OnEventReceived(FGameplayEventData PayloadData)
{
}

void URPGGA_Monster_Attack_EarthQuake::OnEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URPGGA_Monster_Attack_EarthQuake::CheckBoxTrace(FGameplayEventData PayloadData)
{
	FVector StartLocation = GetMonsterCharacterFromActorInfo()->GetActorLocation();
	FVector EndLocation = GetMonsterCharacterFromActorInfo()->GetActorForwardVector() * 100.f;
	FRotator EndRotation = UKismetMathLibrary::MakeRotFromX(GetMonsterCharacterFromActorInfo()->GetActorForwardVector());
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;

	TArray<FHitResult> OUTHitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(this, StartLocation, EndLocation, FVector(500.f, 500.f, 200.f), EndRotation,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OUTHitResults, true,FColor::Green, FColor::Red, 2.0f);

	FGameplayEffectSpecHandle SpecHandle = MakeMonsterBaseDamageEffectSpecHandle(DamageEffectClass, DamageScale);
	
	ApplyGameplayEffectgSpecHandleToHitResults(SpecHandle, OUTHitResults);
}

void URPGGA_Monster_Attack_EarthQuake::ApplyDamage(FGameplayEventData PayloadData)
{
}
