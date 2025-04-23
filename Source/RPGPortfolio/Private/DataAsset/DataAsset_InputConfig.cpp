// LJS


#include "DataAsset/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionGAs(const FGameplayTag& InInputTag) const
{
	for (const FInputActionGA& InputActionGA : NativeInputActionGAs)
	{
		if (InputActionGA.InputTag == InInputTag && InputActionGA.InputAction)
		{
			return InputActionGA.InputAction;
		}
	}

	return nullptr;
}
