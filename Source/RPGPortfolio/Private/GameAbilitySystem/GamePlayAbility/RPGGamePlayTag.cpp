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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_FirstSkill_Keyboard, "InputTag.FirstSkill.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SecondSkill_Keyboard, "InputTag.SecondSkill.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ThirdSkill_Keyboard, "InputTag.ThirdSkill.Keyboard");
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
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_AttackBuff, "Player.Ability.Skill.AttackBuff");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_NenShot, "Player.Ability.Skill.NenShot");

	// Player - Status
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanHeavyAttackCombo, "Player.Status.CanHeavyAttackCombo");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanParryingAttack, "Player.Status.CanParryingAttack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ActiveAttackBuff, "Player.Status.ActiveAttackBuff");

	// Player - Status - ActionState
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ActionState_IsAttacking, "Player.Status.ActionState.IsAttacking");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ActionState_IsJumping, "Player.Status.ActionState.IsJumping");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ActionState_IsSkilling, "Player.Status.ActionState.IsSkilling");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ActionState_IsDefensing, "Player.Status.ActionState.IsDefensing");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ActionState_IsDodging, "Player.Status.ActionState.IsDodging");
	
	// Player - Status - AttributeSet
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_AttributeSet_MpNotFull, "Player.Status.AttributeSet.MpNotFull");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_AttributeSet_MpFull, "Player.Status.AttributeSet.MpFull");

	// Player - Status - Cooldown
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CoolDown_NenShot, "Player.Status.CoolDown.NenShot");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CoolDown_AttackBuff, "Player.Status.CoolDown.AttackBuff");
	
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_DefenseSuccess, "Player.Event.DefenseSuccess");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ActiveBuff, "Player.Event.ActiveBuff");

	
	//Monster
	UE_DEFINE_GAMEPLAY_TAG(Monster_Weapon, "Monster.Weapon");
	
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Melee, "Monster.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Ranged, "Monster.Ability.Attack.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_EarthQuake, "Monster.Ability.Attack.EarthQuake");

	UE_DEFINE_GAMEPLAY_TAG(Monster_Status_Strafe, "Monster.Status.Strafe");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Status_IsBeingAttacked, "Monster.Status.IsBeingAttacked");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Status_IsBeingUndefendableAttacked, "Monster.Status.IsBeingUndefendableAttacked");
	// Event

	
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_AttackHit_Melee, "Character.Event.AttackHit.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_AttackHit_AreaAttack, "Character.Event.AttackHit.AreaAttack");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_HitReact, "Character.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_SpawnProjectile, "Character.Event.SpawnProjectile");

	
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

	//Level
	UE_DEFINE_GAMEPLAY_TAG(Level_Menu_MainMap, "Level.Menu.MainMap");
	UE_DEFINE_GAMEPLAY_TAG(Level_Game_Town, "Level.Game.Town");
	UE_DEFINE_GAMEPLAY_TAG(Level_Game_Dungeon, "Level.Game.Dungeon");
	
	//GamePlayCue
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Light, "GameplayCue.Player.Fighter.Sound.AttackHit.Melee.Light");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Sound_AttackHit_Melee_Heavy, "GameplayCue.Player.Fighter.Sound.AttackHit.Melee.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Sound_AttackHit_Parrying, "GameplayCue.Player.Fighter.Sound.AttackHit.Parrying");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_Defense, "GameplayCue.Player.Fighter.Effect.Defense");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_DefenseSucess, "GameplayCue.Player.Fighter.Effect.DefenseSucess");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_DefenseParrying, "GameplayCue.Player.Fighter.Effect.DefenseParrying");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Fighter_Effect_ApplyBuff, "GameplayCue.Player.Fighter.Effect.ApplyBuff");
	
}