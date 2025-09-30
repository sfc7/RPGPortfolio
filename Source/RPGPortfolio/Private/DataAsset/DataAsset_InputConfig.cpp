// LJS


#include "DataAsset/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionGAs(const FGameplayTag& InInputTag) const
{
	if (!InInputTag.IsValid()) return nullptr;

	// 네이티브 입력 액션 배열에서 검색
	for (const FInputActionGA& InputActionGA : NativeInputActionGAs)
	{
		if (!InputActionGA.IsValid()) continue;

		// FGameplayTag 매칭 확인
		const bool bTagMatches = (InputActionGA.InputTag == InInputTag);
		if (bTagMatches)
		{
			return InputActionGA.InputAction;
		}
	}

	return nullptr;
}
