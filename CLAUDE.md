# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Unreal Engine 5.5 기반 3D ARPG 포트폴리오 프로젝트. C++ 중심으로 작성되며 Blueprint는 보조적으로 사용.

- **엔진**: Unreal Engine 5.5
- **모듈**: `RPGPortfolio` (단일 런타임 모듈)
- **핵심 플러그인**: GameplayAbilities, MotionWarping
- **핵심 모듈 의존성**: EnhancedInput, GameplayTags, GameplayTasks, Niagara, NavigationSystem, MoviePlayer

## Build Commands

```bash
# Unreal Build Tool로 빌드 (Development Editor)
UnrealBuildTool.exe RPGPortfolio Win64 Development -Project="RPGPortfolio.uproject"

# 에디터에서 빌드: Ctrl+Shift+B (VS) 또는 에디터 내 Compile 버튼
```

C++ 파일 추가 시 반드시 `RPGPortfolio.Build.cs`의 모듈 의존성을 확인할 것. GAS 관련 클래스는 `GameplayAbilities` 모듈이 `.uproject`의 `AdditionalDependencies`에 있으나, `Build.cs`에는 명시되어 있지 않으므로 새 GAS 타입 사용 시 추가 필요.

## Architecture

### 캐릭터 계층 구조

```
ARPGCharacterBase  (IAbilitySystemInterface, IUIInterface, IUInteractionInterface)
├── APlayerCharacterBase
│   └── APlayerCharacter_Fighter
├── AMonsterCharacter
└── ARPGNPCCharacterBase
```

모든 캐릭터는 `URPGAbilitySystemComponent`와 `URPGAttributeSet`을 보유. `CharacterStartUpData` (TSoftObjectPtr<UDataAsset_AbilitySetBase>)로 시작 어빌리티/이펙트를 데이터 드리븐으로 주입.

### Gameplay Ability System (GAS)

**태그 시스템**: 모든 GameplayTag는 `RPGGameplayTag` C++ 네임스페이스로 타입 세이프하게 선언됨 (`RPGGamePlayTag.h`). 문자열 리터럴 사용 금지 - 반드시 네임스페이스 상수 사용.

**어빌리티 계층**:
```
URPGGameplayAbility
└── UPlayerGameplayAbility
│   └── UPlayerCombatGameplayAbility  ← 공격/방어/회피 계열
└── UMonsterGameplayAbility
```

**어빌리티 활성화 정책** (`ERPGGameplayAbilityActivationType`):
- `OnTriggered`: 입력/이벤트로 수동 활성화
- `OnGiven`: ASC에 부여 즉시 자동 활성화 (패시브)

**동적 어빌리티**: `DynamicSpecSourceTags`가 있는 어빌리티만 Save/Load 대상. `URPGAbilitySystemComponent::IsDynamicAbility()`로 판별.

**입력 → 어빌리티 흐름**:
1. `UPlayerEnhancedInputComponent::BindAbilityInputAction()` → InputTag 바인딩
2. `URPGAbilitySystemComponent::OnAbilityInputPressed(InputTag)` → 태그 매칭 어빌리티 활성화

### 데이터 에셋 패턴

`UDataAsset_AbilitySetBase::GiveAbilitySystemComponent(ASC)` 호출 한 번으로:
- `ActivateOnGivenAbilities` → 즉시 활성화 어빌리티 부여
- `ReactiveAbilities` → 반응형 어빌리티 부여
- `InitGameplayEffects` → 초기 스탯 이펙트 적용

### 전투 시스템

`UCombatComponentBase` (캐릭터에 Attach된 Component):
- `TMap<FGameplayTag, AWeaponBase*>` 로 무기를 태그로 관리
- `ToggleCurrentWeaponCollision()` / `ToggleCarriedWeaponCollision()` 으로 히트 감지 ON/OFF
- `OverlappedActors` 배열로 한 스윙에 동일 대상 중복 히트 방지

패링 성공 시 `UPlayerCombatComponent::OnParryingStateChange()`가 IMC(Input Mapping Context)를 동적으로 추가(priority 1)하여 이후 입력 처리를 다르게 함.

### 인벤토리 Strategy Pattern

`UPlayerInventoryComponent`는 두 개의 Strategy를 조합하여 동작:
- `EInventoryTypeStrategy`: Default / Equipment / Potion / Material (어떤 슬롯 배열을 볼 것인가)
- `EInventorySituationStrategy`: Default / InOpenStore / InOpenStorage / InOpenEquipment (어떤 컨텍스트인가)

새로운 인벤토리 동작을 추가할 때는 상속 추가가 아닌 Strategy 구현체 추가로 확장.

### 오브젝트 풀

`UObjectPoolComponent` (플레이어에만 있음):
- `TMap<FGameplayTag, FProjectilePoolData>` - 스킬별 독립 풀
- `SpawnFromPool(Tag)` → 비활성 `APooledActor` 반환 또는 신규 생성
- 스킬 프로젝타일은 반드시 `APooledActor`를 상속하고 `SetIsUse(false)`로 반환

### 퀘스트 시스템 3계층

```
UQuestManager (GameInstanceSubsystem)
└── ARPGQuestSystemActor  ← 퀘스트 인스턴스
    └── FQuest
        └── FStageDetail[]
            └── FObjectiveDetail[]  ← ObjectiveID로 추적
```

`ARPGQuestSystemActor::OnObjectiveIDHeard(FString)` → 목표 진행 증가 → 완료 시 `FOnObjectiveHeard` 델리게이트 발행.

### GameInstanceSubsystem 3종

전역 상태 관리자로 `UGameInstance::GetSubsystem<T>()`로 접근:
- `ULevelManager`: 레벨 로드 + 전체 Save/Load 통합
- `UQuestManager`: 퀘스트 생명주기 및 진행 추적
- `UItemManager`: 아이템 슬롯 조작 및 드래그앤드롭 처리

### AI 구조

`ARPGAIController` → Behavior Tree 실행:
- **BTTask**: `BTTask_ActivateGameAbility` - BT에서 직접 GAS 어빌리티 실행
- **BTService**: `BTService_MW_UpdateTargetActor` - MotionWarp 타겟 갱신
- **BTDeco**: `BTDeco_CheckGamePlayTag` - GAS 태그 기반 실행 조건

## 코딩 컨벤션

- 클래스/함수 주석: `//@ 설명` 형식
- 멤버 변수 prefix: `_` 없음 (파라미터에는 `_` prefix)
- GameplayTag 참조: 항상 `RPGGameplayTag::` 네임스페이스 사용
- 인터페이스 메서드 접근: `IUIInterface::Execute_GetUIComponent()` 패턴
- 새 캐릭터 타입: 반드시 `ARPGCharacterBase` 상속 및 `CreateDefaultAttributeSet()` 오버라이드

## 주요 파일 위치

| 용도 | 경로 |
|------|------|
| 모든 GameplayTag 선언 | `Public/GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h` |
| 캐릭터 베이스 | `Public/Character/RPGCharacterBase.h` |
| 커스텀 ASC | `Public/GameAbilitySystem/RPGAbilitySystemComponent.h` |
| 어빌리티 베이스 | `Public/GameAbilitySystem/GamePlayAbility/RPGGameplayAbility.h` |
| 어빌리티셋 DataAsset | `Public/DataAsset/DataAsset_AbilitySetBase.h` |
| 전투 컴포넌트 베이스 | `Public/Component/CombatComponentBase.h` |
| 인벤토리 | `Public/Component/Player/PlayerInventoryComponent.h` |
| 퀘스트 매니저 | `Public/GameMode/GameManager/QuestManager.h` |
| 레벨/세이브 매니저 | `Public/GameMode/GameManager/LevelManager.h` |
