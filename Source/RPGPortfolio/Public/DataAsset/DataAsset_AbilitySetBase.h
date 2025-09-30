// LJS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"
#include "DataAsset_AbilitySetBase.generated.h"

class URPGGameplayAbility;
class URPGAbilitySystemComponent;
class UGameplayEffect;

/**
 * UDataAsset_AbilitySetBase
 * 
 * AbilitySet의 기본 데이터 에셋 Base
 * 캐릭터에게 부여할 어빌리티들 정의, 즉시 활성화 어빌리티와 반응형 어빌리티 구분,
 * 초기 게임플레이 이펙트 적용
 */
UCLASS()
class RPGPORTFOLIO_API UDataAsset_AbilitySetBase : public UDataAsset
{
	GENERATED_BODY()

public:
	//@ ASC에 어빌리티들과 초기 이펙트 부여
	virtual void GiveAbilitySystemComponent(URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
 
protected:
	//@ 부여 시 즉시 활성화되는 어빌리티들, 패시브 스킬 등
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray< TSubclassOf <URPGGameplayAbility>> ActivateOnGivenAbilities;

	//@ 반응형 어빌리티들, 특정 조건 트리거 발동형
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray< TSubclassOf <URPGGameplayAbility>> ReactiveAbilities;

	//@ 초기에 적용할 GameplayEffect들 
	UPROPERTY(EditDefaultsOnly, Category = "GA")
	TArray<TSubclassOf<UGameplayEffect>> InitGameplayEffects;

	//@ 어빌리티 배열을 ASC에 부여하는 헬퍼 함수
	void GrantAbilities(const TArray< TSubclassOf <URPGGameplayAbility>>& _ToBeGrantedAbilities,URPGAbilitySystemComponent* _ASC,int32 ApplyLevel = 1);
};

