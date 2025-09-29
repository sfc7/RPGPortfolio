// LJS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceBase.h"
#include "PlayerCharacterLinkedAnimLayer.generated.h"

/**
 * UPlayerCharacterLinkedAnimLayer
 *
 * PlayerCharacter를 위한 Linked Anim Layer
 * - AnimGraph에서 Layer 인터페이스를 정의할 때 사용
 * - 애니메이션 레이어별 로직 확장을 위한 클래스
 */
UCLASS()
class RPGPORTFOLIO_API UPlayerCharacterLinkedAnimLayer : public UAnimInstanceBase
{
	GENERATED_BODY()
	
};
