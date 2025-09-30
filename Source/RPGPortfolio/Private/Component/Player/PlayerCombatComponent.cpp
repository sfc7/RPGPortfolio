// LJS

#include "Component/Player/PlayerCombatComponent.h"
#include "WorldStatic/Weapon/PlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "RPGFunc.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitRegisterGameplayTagEvent();
}


APlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag _WeaponTagToGet) const
{
	AWeaponBase* const BaseWeapon = GetCharacterCarriedWeaponByTag(_WeaponTagToGet);
	
	return Cast<APlayerWeapon>(BaseWeapon);
}


APlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	AWeaponBase* const BaseWeapon = GetCharacterCurrentEquippedWeapon();
	
	return Cast<APlayerWeapon>(BaseWeapon);
}


float UPlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel() const
{
	const APlayerWeapon* const CurrentWeapon = GetPlayerCurrentEquippedWeapon();
	if (!IsValid(CurrentWeapon))
	{
		return 0.0f;
	}

	return CurrentWeapon->WeaponDefaultData.WeaponBaseDamage;
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType, FName SocketName)
{
	if (!IsValid(_HitActor))
	{
		return;
	}

	// 중복 히트 방지
	if (OverlappedActors.Contains(_HitActor))
	{
		return;
	}

	// 히트된 액터를 중복 히트 방지 목록에 추가
	OverlappedActors.AddUnique(_HitActor);

	// 소켓 위치 계산
	FVector SocketLocation = FVector::ZeroVector;
	const APlayerCharacterBase* PlayerCharacter = GetOwningPawn<APlayerCharacterBase>();
	
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	// 메시 컴포넌트에서 소켓 위치 가져오기
	const USkeletalMeshComponent* MeshComp = PlayerCharacter->GetMesh();
	if (IsValid(MeshComp) && SocketName != NAME_None)
	{
		SocketLocation = MeshComp->GetSocketLocation(SocketName);
	}
	
	// 커스텀 히트 결과 생성
	FHitResult CustomHitResult;
	CustomHitResult.Location = SocketLocation;
	
	// 정규화
	const FVector DirectionVector = (SocketLocation - GetOwningPawn()->GetActorLocation()).GetSafeNormal();
	CustomHitResult.Normal = DirectionVector;
	
	const URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}

	// 근접 공격 관련 GameplayEffect를 EveneData에 담아서 Event로 전달
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddHitResult(CustomHitResult, true); 
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = _HitActor;
	EventData.EventMagnitude = WeaponAttackRate;
	EventData.ContextHandle = EffectContext;
	

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		RPGGameplayTag::Character_Event_AttackHit_Melee,
		EventData
	);

	// 강공격 또는 패링 공격인 경우 히트 일시정지 이벤트 발생
	const bool bIsHeavyOrParryingAttack = (AttackType == EWeaponAttackType::Heavy || AttackType == EWeaponAttackType::Parrying);
	if (bIsHeavyOrParryingAttack)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			RPGGameplayTag::Player_Event_HitPause,
			FGameplayEventData()
		);
	}
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* _HitActor, float WeaponBaseDamage, float WeaponAttackRate, EWeaponAttackType AttackType, EWeaponType WeaponType)
{
	// 강공격인 경우에만 히트 일시정지 이벤트 발생
	const bool bIsHeavyAttack = (AttackType == EWeaponAttackType::Heavy);
	if (bIsHeavyAttack)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			RPGGameplayTag::Player_Event_HitPause,
			FGameplayEventData()
		);
	}
}

void UPlayerCombatComponent::OnParryingStateChange(const FGameplayTag CallbackTag, int32 NewCount)
{	
	const APlayerController* PC = GetOwningController<APlayerController>();
	if (!IsValid(PC))
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!IsValid(Subsystem))
	{
		return;
	}
	
	const APlayerCharacterBase* PlayerCharacter = GetOwningPawn<APlayerCharacterBase>();
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	const UInputMappingContext* ParryingContext = PlayerCharacter->GetParryingInputMappingContext();
	if (!ParryingContext)
	{
		return;
	}

	// 패링 상태에 따른 입력 매핑 컨텍스트 추가/제거
	const bool bCanParry = (NewCount > 0);
	if (bCanParry)
	{
		// 패링 가능 상태 - 패링 전용 입력 매핑 활성화
		Subsystem->AddMappingContext(ParryingContext, MappingPriority);
	}
	else
	{
		// 패링 불가능 상태 - 패링 전용 입력 매핑 비활성화
		Subsystem->RemoveMappingContext(ParryingContext);
	}
}

void UPlayerCombatComponent::InitRegisterGameplayTagEvent()
{
	URPGAbilitySystemComponent* ASC = GetRPGAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}

	// 패링 가능 상태 태그 이벤트 등록, NewOrRemoved 이벤트로 패링 상태 변화 감지
	ASC->RegisterGameplayTagEvent(
		RPGGameplayTag::Player_Status_CanParryingAttack,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(this, &ThisClass::OnParryingStateChange);
}
