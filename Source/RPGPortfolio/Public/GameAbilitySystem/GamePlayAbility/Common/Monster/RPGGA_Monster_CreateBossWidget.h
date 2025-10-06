// LJS

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/GamePlayAbility/Common/Monster/MonsterGameplayAbility.h"
#include "RPGGA_Monster_CreateBossWidget.generated.h"

/**
 * URPGGA_Monster_CreateBossWidget
 * 
 * 보스 몬스터 관련 HUD를 보여주는 GameplayAbiltiy
 */
UCLASS()
class RPGPORTFOLIO_API URPGGA_Monster_CreateBossWidget : public UMonsterGameplayAbility
{
	GENERATED_BODY()
public:
	URPGGA_Monster_CreateBossWidget();
};
