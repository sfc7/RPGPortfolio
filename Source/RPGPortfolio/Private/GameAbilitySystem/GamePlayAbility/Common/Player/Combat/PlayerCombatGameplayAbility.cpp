// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/Combat/PlayerCombatGameplayAbility.h"

#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/MonsterCharacter.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "GameAbilitySystem/GameplayTask/Player/RPGAT_Player_RotateTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerCombatGameplayAbility::UPlayerCombatGameplayAbility()
{
}

bool UPlayerCombatGameplayAbility::FindNearestEnemyBeforeAttack(float BoxExtent)
{
	// 몬스터 탐지용 박스 트레이스 설정 및 실행
	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		FVector(BoxExtent, BoxExtent, BoxExtent) / 2,
		GetPlayerCharacterFromActorInfo()->GetActorRotation(),
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	// 히트 결과에서 몬스터 찾기
	TArray<AActor*> FindEnemies;
	for (const FHitResult& HitResult : HitResults)
	{
		if (AMonsterCharacter* HitMonster =  Cast<AMonsterCharacter>(HitResult.GetActor()))
		{
			URPGAbilitySystemComponent* MonsterASC = CastChecked<URPGAbilitySystemComponent>(
			   UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitMonster));
            
			if (!MonsterASC->HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead))
			{
				FindEnemies.AddUnique(HitMonster);
			}
		}
	}

	// 적이 없는 경우 false 반환, 있으면 가장 가까운 적 탐지
	if (FindEnemies.IsEmpty())
	{
		return false;
	}
	else
	{
		float ClosetDistance = 0.f;
		RotateTargetActor = UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), FindEnemies, ClosetDistance);
		FindRotation = UKismetMathLibrary::FindLookAtRotation(
				GetPlayerCharacterFromActorInfo()->GetActorLocation(),
				RotateTargetActor->GetActorLocation()
				);
	}

	return true;
}

void UPlayerCombatGameplayAbility::RotateTargetTickBeforeAttack(float DeltaTime)
{
	// 플레이어가 죽은 상태인 경우 탐지 X
	if (HasMatchingGameplayTag(RPGGameplayTag::Character_Status_Dead)) return;
	
	if (!IsValid(RotateTargetActor)) return;

	// 현재 회전에서 목표 회전으로 보간
	const FRotator TargetRot = FMath::RInterpTo(GetPlayerCharacterFromActorInfo()->GetActorRotation(),FindRotation,DeltaTime,15.0f);

	GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f,TargetRot.Yaw,0.f));
}

void UPlayerCombatGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayerCharacterBase* OwnerCharacter = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!IsValid(OwnerCharacter)) return;

	// 기본적으로 전투 어빌리티들은 실행동안 움직임 불가능(경우에 따라 다름)
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UPlayerCombatGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	APlayerCharacterBase* OwnerCharacter = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!IsValid(OwnerCharacter)) return;

	// 어빌리티가 끝나면 움직임 가능한 상태로 바꿈
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
