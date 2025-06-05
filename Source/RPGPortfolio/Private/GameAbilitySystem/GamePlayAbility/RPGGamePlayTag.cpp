// LJS


#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

namespace RPGGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move_Keyboard, "InputTag.Move.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look_Mouse, "InputTag.Look.Mouse");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump_Keyboard, "InputTag.Jump.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Keyboard, "InputTag.LightAttack.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Keyboard, "InputTag.HeavyAttack.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ParryingAttack_Keyboard, "InputTag.ParryingAttack.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ShowDebug_Keyboard, "InputTag.ShowDebug.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dodge_Keyboard, "InputTag.Dodge.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold, "InputTag.Hold");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold_Defense_Keyboard, "InputTag.Hold.Defense.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle");

	
	//Player
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Hand_l, "Player.Weapon.Fighter.Hand.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Hand_r, "Player.Weapon.Fighter.Hand.r");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Foot_l, "Player.Weapon.Fighter.Foot.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Foot_r, "Player.Weapon.Fighter.Foot.r");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light, "Player.Ability.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy, "Player.Ability.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, "Player.Ability.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dodge, "Player.Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Defense, "Player.Ability.Defense");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Parry, "Player.Ability.Attack.Parry");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanHeavyAttackCombo, "Player.Status.CanHeavyAttackCombo");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Dodge, "Player.Status.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Defense, "Player.Status.Defense");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanParryingAttack, "Player.Status.CanParryingAttack");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_DefenseSuccess, "Player.Event.DefenseSuccess");
	
	//Monster
	UE_DEFINE_GAMEPLAY_TAG(Monster_Weapon, "Monster.Weapon");
	
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Melee, "Monster.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Ranged, "Monster.Ability.Attack.Ranged");

	UE_DEFINE_GAMEPLAY_TAG(Monster_Status_Strafe, "Monster.Status.Strafe");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Status_IsBeingAttacked, "Monster.Status.IsBeingAttacked");
	// Event
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_AttackHit_Melee, "Character.Event.AttackHit.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_HitReact, "Character.Event.HitReact");

	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact, "Character.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Death, "Character.Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Dead, "Character.Status.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_HitReact_Front, "Character.Status.HitReact.Front");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_HitReact_Back, "Character.Status.HitReact.Back");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_HitReact_Left, "Character.Status.HitReact.Left");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_HitReact_Right, "Character.Status.HitReact.Right");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Invincible, "Character.Status.Invincible");
	
	
	// Data
	UE_DEFINE_GAMEPLAY_TAG(Data_Value_SetByCaller_BaseDamage, "Data.Value.SetByCaller.BaseDamage");

	UE_DEFINE_GAMEPLAY_TAG(Data_DamageType_SetByCaller_Light, "Data.DamageType.SetByCaller.Light");
	UE_DEFINE_GAMEPLAY_TAG(Data_DamageType_SetByCaller_Heavy, "Data.DamageType.SetByCaller.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Data_DamageType_SetByCaller_Parrying, "Data.DamageType.SetByCaller.Parrying");

	//GamePlayCue
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Light, "GameplayCue.Player.Fighter.Sound.AttackHit.Melee.Light");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Heavy, "GameplayCue.Player.Fighter.Sound.AttackHit.Melee.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Sound_AttackHit_Parrying, "GameplayCue.Player.Fighter.Sound.AttackHit.Parrying");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_Defense, "GameplayCue.Player.Fighter.Effect.Defense");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_DefenseSucess, "GameplayCue.Player.ighter.Effect.DefenseSucess");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_DefenseParrying, "GameplayCue.Player.Fighter.Effect.DefenseParrying");
	
	
}