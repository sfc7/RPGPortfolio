// LJS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

class URPGSaveGame;
/**
 * URPGAbilitySystemComponent
 * 
 * GAS를 사용하는 액터들이 모두 사용할 Component
 */
UCLASS()
class RPGPORTFOLIO_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//@ 입력 태그에 따른 어빌리티 활성화 처리
	void OnAbilityInputPressed(const FGameplayTag& _InputTag);
	
	//@ 입력 태그에 따른 어빌리티 비활성화 처리 
	void OnAbilityInputReleased(const FGameplayTag& _InputTag);

	//@ GameplayTag로 어빌리티 활성화
	UFUNCTION(BlueprintCallable)
	bool ActivateGamePlayAbilityByTag(FGameplayTag _ActivateAbilityTag);

	//@ 활성화된 어빌리티를 GameplayTag로 검색
	FGameplayAbilitySpec* FindActiveAbilityByTag(FGameplayTag AbilityTag);

	//@ 활성화된 모든 스킬 어빌리티 반환
	UFUNCTION(BlueprintPure, Category = "Skills")
	TArray<FGameplayAbilitySpec> GetAllSkillAbilities() const;

	//@ 동적 어빌리티들을 세이브 게임에 저장 
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveDynamicAbilitiesToSaveGame(URPGSaveGame* SaveGame);
	
	//@ 세이브 게임에서 동적 어빌리티들 로드 
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadDynamicAbilitiesFromSaveGame(URPGSaveGame* SaveGame);

private:
	//@ 어빌리티가 동적 어빌리티인지 확인 (DynamicSpecSourceTags 존재 여부)
	bool IsDynamicAbility(const FGameplayAbilitySpec& Spec) const;
};
