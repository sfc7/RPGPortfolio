// LJS

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "PlayerEnhancedInputComponent.generated.h"


/**
 * UPlayerEnhancedInputComponent
 * 
 * Input 관련된 일들을 ASC로 처리하기 위한 컴포넌트
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	//@ 네이티브 입력 액션 바인딩 (이동, 시점 조작 등)
	template<class UserObject,typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,const FGameplayTag& InInputTag,ETriggerEvent TriggerEvent,UserObject* ContextObject,CallbackFunc Func);

	//@ 어빌리티 입력 바인딩 (스킬, 공격 등)
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
			// 입력 액션 바인딩
			BindAction(FoundInputAction,TriggerEvent,ContextObject,Func);
		}
	}
}

template <class UserObject, typename CallbackFunc>
void UPlayerEnhancedInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc PressFunc, CallbackFunc ReleaseFunc)
{
	if(IsValid(InInputConfig))
	{
		// 모든 어빌리티 입력 액션에 대해 바인딩
		for (const FInputActionGA& AbilityInputAction : InInputConfig->AbilityInputActionGAs)
		{
			if (AbilityInputAction.IsValid())
			{
				// 입력 시작 시 호출될 함수 바인딩 (Press)
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Started, ContextObject, PressFunc, AbilityInputAction.InputTag);
				// 입력 완료 시 호출될 함수 바인딩 (Release)
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Completed, ContextObject, ReleaseFunc, AbilityInputAction.InputTag);
			}
		}
	}
}

