// LJS

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "PlayerEnhancedInputComponent.generated.h"


//TEst333
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject,typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,const FGameplayTag& InInputTag,ETriggerEvent TriggerEvent,UserObject* ContextObject,CallbackFunc Func);

	template<class UserObject,typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,CallbackFunc PressFunc, CallbackFunc ReleaseFunc);
};

template<class UserObject, typename CallbackFunc>
inline void UPlayerEnhancedInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	if(IsValid(InInputConfig))
	{
		if (UInputAction* FoundInputAction = InInputConfig->FindNativeInputActionGAs(InInputTag))
		{
			BindAction(FoundInputAction,TriggerEvent,ContextObject,Func);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("InputConfig invalid"));
}

template <class UserObject, typename CallbackFunc>
void UPlayerEnhancedInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc PressFunc, CallbackFunc ReleaseFunc)
{
	if(IsValid(InInputConfig))
	{
		for (const FInputActionGA& AbilityInputAction : InInputConfig->AbilityInputActionGAs)
		{
			if (AbilityInputAction.IsValid())
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Started, ContextObject, PressFunc, AbilityInputAction.InputTag);
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Completed, ContextObject, ReleaseFunc, AbilityInputAction.InputTag);
			}
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("InputConfig invalid"));


}

