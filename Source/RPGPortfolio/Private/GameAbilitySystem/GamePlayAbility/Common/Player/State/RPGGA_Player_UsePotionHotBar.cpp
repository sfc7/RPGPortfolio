// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_UsePotionHotBar.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "Component/InventoryComponent.h"
#include "DataAsset/Item/DataAsset_RPGItemData_Potion.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h"
#include "GameAbilitySystem/RPGAbilitySystemComponent.h"
URPGGA_Player_UsePotionHotBar::URPGGA_Player_UsePotionHotBar()
{
}

void URPGGA_Player_UsePotionHotBar::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	int32 SlotIndex = -1;

	// 어빌리티 스펙에서 슬롯 인덱스 추출
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			const FGameplayTagContainer& Tags = AbilitySpec->GetDynamicSpecSourceTags();

			// 입력 태그에 따른 슬롯 인덱스 설정
			for (const FGameplayTag& Tag : Tags)
			{
				if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_1_Keyboard) SlotIndex = 0;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_2_Keyboard) SlotIndex = 1;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_3_Keyboard) SlotIndex = 2;
			}
		}
	}
	
	// 플레이어 캐릭터 및 핫바 인벤토리 가져오기
	APlayerCharacterBase* Player = GetPlayerCharacterFromActorInfo();
	UInventoryComponent* HotBarInventory = Player->GetPlayerPotionHotBar();
    
	if (!IsValid(HotBarInventory) || !HotBarInventory->IsValidSlotIndex(SlotIndex))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	// 대상 슬롯 및 포션 데이터 가져오기
	FInventorySlot& TargetSlot = HotBarInventory->DefaultItemSlots[SlotIndex];
	UDataAsset_RPGItemData_Potion* PotionData = Cast<UDataAsset_RPGItemData_Potion>(TargetSlot.ItemDataAsset.LoadSynchronous());
    
	if (!IsValid(PotionData))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	// 힐 이펙트 적용
	ApplyHealEffect(PotionData->HealAmount);

	// 포션 수량 처리
	if (TargetSlot.Quantity > 1)
	{
		// 수량이 1개 이상인 경우 1개 감소
		FInventorySlot ModifiedSlot = TargetSlot;
		HotBarInventory->SetQuantityAtSlot(ModifiedSlot, TargetSlot.Quantity - 1);
	}
	else
	{
		// 수량이 1개인 경우 슬롯에서 제거
		HotBarInventory->RemoveItemToIndex(SlotIndex);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void URPGGA_Player_UsePotionHotBar::EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 포션 사용 이펙트용 FGameplayTag 큐 제거
	GetPlayerCharacterFromActorInfo()->GetRPGAbilitySystemComponent()->RemoveGameplayCue(PotionUseFXGameplayCue);
}

bool URPGGA_Player_UsePotionHotBar::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	int32 SlotIndex = -1;

	// 어빌리티 스펙에서 슬롯 인덱스 추출
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			const FGameplayTagContainer& Tags = AbilitySpec->GetDynamicSpecSourceTags();

			// 입력 태그에 따른 슬롯 인덱스 설정
			for (const FGameplayTag& Tag : Tags)
			{
				if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_1_Keyboard) SlotIndex = 0;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_2_Keyboard) SlotIndex = 1;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_3_Keyboard) SlotIndex = 2;
			}
		}
	}

	// 아이템 유효성 검사 과정
	const APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!IsValid(Player)) return false;

	const UInventoryComponent* HotBar = Player->GetPlayerPotionHotBar();
	if (!IsValid(HotBar) || !Player->GetPlayerPotionHotBar()->IsValidSlotIndex(SlotIndex)) return false;

	const FInventorySlot& Slot = HotBar->DefaultItemSlots[SlotIndex];
	if (Slot.ItemDataAsset.IsNull()) return false;

	const UDataAsset_RPGItemData* ItemData = Slot.ItemDataAsset.LoadSynchronous();
	if (!ItemData || ItemData->ItemType != EItemType::Potion) return false;

	// 수량 검사
	if (Slot.Quantity <= 0) return false;

	// 포션 사용 이펙트용 FGameplayTag 큐 ASC에 추가
	FGameplayCueParameters GCEffectParam;
	GCEffectParam.TargetAttachComponent = GetOwningComponentFromActorInfo();
	ActorInfo->AbilitySystemComponent->AddGameplayCue(PotionUseFXGameplayCue, GCEffectParam);
	
	return true;
}

void URPGGA_Player_UsePotionHotBar::ApplyHealEffect(int32 HealAmount)
{
	if (!IsValid(PotionHealEffectClass)) return;

	// 포션 사용 데이터 처리할 GameplayEffect 클래스 생성
	FGameplayEffectContextHandle EffectContext = GetRPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	
	FGameplayEffectSpecHandle SpecHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		PotionHealEffectClass, 
		1.0f, 
		EffectContext
	);

	// 힐 양 설정
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(
	  FGameplayTag::RequestGameplayTag(FName("Data.Value.SetByCaller.UsePotion")), 
	  HealAmount
  );

	// 플레이어에게 힐 적용
	GetRPGAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}