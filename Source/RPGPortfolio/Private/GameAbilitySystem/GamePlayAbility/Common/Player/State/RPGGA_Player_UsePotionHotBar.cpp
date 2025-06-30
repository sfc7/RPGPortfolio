// LJS


#include "GameAbilitySystem/GamePlayAbility/Common/Player/State/RPGGA_Player_UsePotionHotBar.h"
#include "Character/Player/PlayerCharacterBase.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "Component/Player/PlayerInventoryComponent.h"
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

	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			const FGameplayTagContainer& Tags = AbilitySpec->GetDynamicSpecSourceTags();
        
			for (const FGameplayTag& Tag : Tags)
			{
				if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_1_Keyboard) SlotIndex = 0;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_2_Keyboard) SlotIndex = 1;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_3_Keyboard) SlotIndex = 2;
			}
		}
	}

	APlayerCharacterBase* Player = GetPlayerCharacterFromActorInfo();
	UPlayerInventoryComponent* HotBarInventory = Player->GetPlayerPotionHotBar();
    
	if (!HotBarInventory || !HotBarInventory->IsValidSlotIndex(SlotIndex))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	FInventorySlot& TargetSlot = HotBarInventory->ItemSlots[SlotIndex];
	UDataAsset_RPGItemData_Potion* PotionData = Cast<UDataAsset_RPGItemData_Potion>(TargetSlot.ItemDataAsset.LoadSynchronous());
    
	if (!PotionData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	ApplyHealEffect(PotionData->HealAmount);
	
	if (TargetSlot.Quantity > 1)
	{
		FInventorySlot ModifiedSlot = TargetSlot;
		HotBarInventory->SetQuantityAtSlot(ModifiedSlot, TargetSlot.Quantity - 1);
	}
	else
	{
		HotBarInventory->RemoveItemToIndex(SlotIndex);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void URPGGA_Player_UsePotionHotBar::EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool URPGGA_Player_UsePotionHotBar::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	
	int32 SlotIndex = -1;

	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			const FGameplayTagContainer& Tags = AbilitySpec->GetDynamicSpecSourceTags();
        
			for (const FGameplayTag& Tag : Tags)
			{
				if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_1_Keyboard) SlotIndex = 0;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_2_Keyboard) SlotIndex = 1;
				else if (Tag == RPGGameplayTag::InputTag_UseHotBarItem_3_Keyboard) SlotIndex = 2;
			}
		}
	}
	
	const APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(ActorInfo->AvatarActor.Get());
	if (!Player) return false;

	const UPlayerInventoryComponent* HotBar = Player->GetPlayerPotionHotBar();
	if (!Player->GetPlayerPotionHotBar() || !Player->GetPlayerPotionHotBar()->IsValidSlotIndex(SlotIndex)) return false;

	const FInventorySlot& Slot = HotBar->ItemSlots[SlotIndex];
	if (Slot.ItemDataAsset.IsNull()) return false;

	const UDataAsset_RPGItemData* ItemData = Slot.ItemDataAsset.LoadSynchronous();
	if (!ItemData || ItemData->ItemType != EItemType::Potion) return false;

	if (Slot.Quantity <= 0) return false;
	
	return true;
}

void URPGGA_Player_UsePotionHotBar::ApplyHealEffect(int32 HealAmount)
{
	if (!PotionHealEffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = GetRPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    
	FGameplayEffectSpecHandle SpecHandle = GetRPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		PotionHealEffectClass, 
		1.0f, 
		EffectContext
	);

	// SetByCaller로 힐량 설정
	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.HealAmount")), HealAmount);

	GetRPGAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void URPGGA_Player_UsePotionHotBar::OnEndAbilityCallback()
{
}
