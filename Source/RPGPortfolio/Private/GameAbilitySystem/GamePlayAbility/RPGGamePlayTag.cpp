// LJS


#include "GameAbilitySystem/GamePlayAbility/RPGGamePlayTag.h"

namespace RPGGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move_Keyboard, "InputTag.Move.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look_Mouse, "InputTag.Look.Mouse");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump_Keyboard, "InputTag.Jump.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Keyboard, "InputTag.LightAttack.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Keyboard, "InputTag.HeavyAttack.Keyboard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ShowDebug_Keyboard, "InputTag.ShowDebug.Keyboard");

	
	//Player
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Hand_l, "Player.Weapon.Fighter.Hand.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Hand_r, "Player.Weapon.Fighter.Hand.r");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Foot_l, "Player.Weapon.Fighter.Foot.l");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Fighter_Foot_r, "Player.Weapon.Fighter.Foot.r");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light, "Player.Ability.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy, "Player.Ability.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, "Player.Ability.HitPause");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanHeavyAttackCombo, "Player.Status.CanHeavyAttackCombo");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");

	//Monster
	UE_DEFINE_GAMEPLAY_TAG(Monster_Weapon, "Monster.Weapon");
	
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Melee, "Monster.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Ranged, "Monster.Ability.Attack.Ranged");
	// Event
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_AttackHit_Melee, "Character.Event.AttackHit.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_HitReact, "Character.Event.HitReact");

	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact, "Character.Ability.HitReact");
	
	// Data
	UE_DEFINE_GAMEPLAY_TAG(Data_Value_SetByCaller_BaseDamage, "Data.Value.SetByCaller.BaseDamage");

	UE_DEFINE_GAMEPLAY_TAG(Data_DamageType_SetByCaller_Light, "Data.DamageType.SetByCaller.Light");
	UE_DEFINE_GAMEPLAY_TAG(Data_DamageType_SetByCaller_Heavy, "Data.DamageType.SetByCaller.Heavy");
	
}