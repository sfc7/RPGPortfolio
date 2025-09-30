// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

FGameplayTag;
class UInputAction;
class UInputMappingContext;

//@ InputAction을 GameplayTag로 관리할 구조체
USTRUCT(BlueprintType)
struct FInputActionGA
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	bool IsValid() const { return InputTag.IsValid() && InputAction; }
};

/**
 * UDataAsset_InputConfig
 * 
 * ASC에 등록할 InputAction 관리 DataAsset
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	//@ 기본 IMC
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultInputMappingContext;

	//@ 네이티브 입력 액션들 (이동, 시점 조작, UI 호출 등)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionGA> NativeInputActionGAs;

	//@ 어빌리티 입력 액션들 (스킬, 공격, 방어 등)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionGA> AbilityInputActionGAs;

	//@ GameplayTag로 네이티브 입력 액션 검색
	UInputAction* FindNativeInputActionGAs(const FGameplayTag& InInputTag) const;

	
};
