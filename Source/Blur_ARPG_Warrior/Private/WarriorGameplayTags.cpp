// Blur Feng All Rights Reserved.


#include "WarriorGameplayTags.h"

namespace WarriorGameplayTags
{
	//Tips：Tag命名规则
	//因为Tag在蓝图中使用时，Tag是按首字母顺序排列的。
	//所以应当按项目需求规范命名规则。
	//比如EquipAxe和UnequipAxe。我们将Axe作为开头的话，能根据具体对象分类排列。
	//而用Equip和Unequip开头，则按照行为分类排列。

	UE_DEFINE_GAMEPLAY_TAG(None, "None");
	
	//** Input Tags **//
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe, "InputTag.EquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe, "InputTag.UnequipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Axe, "InputTag.LightAttack.Axe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Axe, "InputTag.HeavyAttack.Axe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SpecialWeaponAbility_Light, "InputTag.SpecialWeaponAbility.Light");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SpecialWeaponAbility_Heavy, "InputTag.SpecialWeaponAbility.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_PickUp_Stones, "InputTag.PickUp.Stones");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ResetView, "InputTag.ResetView");
	
	//MustBeHeld 持续按住
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld, "InputTag.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Block, "InputTag.MustBeHeld.Block");

	//Toggleable 切换形式输入
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable, "InputTag.Toggleable");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_TargetLock, "InputTag.Toggleable.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_Rage, "InputTag.Toggleable.Rage");


	//**  Identity 表明身份的Tag  **//
	UE_DEFINE_GAMEPLAY_TAG(Identity_Character_Hero, "Identity.Character.Hero");
	UE_DEFINE_GAMEPLAY_TAG(Identity_Enemy, "Identity.Enemy");

	
	//** Player tags **//
	//Ability 技能
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, "Player.Ability.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, "Player.Ability.Unequip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Axe, "Player.Ability.Attack.Light.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, "Player.Ability.Attack.Heavy.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, "Player.Ability.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "Player.Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, "Player.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, "Player.Ability.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Rage, "Player.Ability.Rage");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpecialWeaponAbility_Light, "Player.Ability.SpecialWeaponAbility.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpecialWeaponAbility_Heavy, "Player.Ability.SpecialWeaponAbility.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_PickUp_Stones, "Player.Ability.PickUp.Stones");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_ResetView, "Player.Ability.ResetView");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_JumpTo_Attack_Light_Axe, "Player.Ability.JumpTo.Attack.Light.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_JumpTo_Attack_Light_Axe_Finish, "Player.Ability.JumpTo.Attack.Light.Axe.Finish");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_JumpTo_Attack_Heavy_Axe, "Player.Ability.JumpTo.Attack.Heavy.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_JumpTo_Attack_Heavy_Axe_Finish, "Player.Ability.JumpTo.Attack.Heavy.Axe.Finish");

	//Cooldown 冷却
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_SpecialWeaponAbility_Light, "Player.Cooldown.SpecialWeaponAbility.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_SpecialWeaponAbility_Heavy, "Player.Cooldown.SpecialWeaponAbility.Heavy");

	//Weapon 武器Tag，用于区分和管理不同的武器Actor。
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "Player.Weapon.Axe");

	//Event 事件
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "Player.Event.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "Player.Event.Unequip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SuccessfulBlock, "Player.Event.SuccessfulBlock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Triggered, "Player.Event.SwitchTarget.Triggered");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Completed, "Player.Event.SwitchTarget.Completed");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ActivateRage, "Player.Event.ActivateRage");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AOE, "Player.Event.AOE");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ConsumeStones, "Player.Event.ConsumeStones");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ResetView, "Player.Event.ResetView");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_RepeatInputWhenAbilityActive, "Player.Event.RepeatInputWhenAbilityActive");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Combo_WaitInput, "Player.Event.Combo.WaitInput");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Combo_StartNextAttack, "Player.Event.Combo.StartNextAttack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Combo_Reset, "Player.Event.Combo.Reset");
	
	//Status 状态
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "Player.Status.JumpToFinisher");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling, "Player.Status.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Blocking, "Player.Status.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "Player.Status.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_Activating, "Player.Status.Rage.Activating");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_Active, "Player.Status.Rage.Active");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_Full, "Player.Status.Rage.Full");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_None, "Player.Status.Rage.None");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Vitality_RecoverStop, "Player.Status.Vitality.RecoverStop");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Vitality_Full, "Player.Status.Vitality.Full");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Vitality_None, "Player.Status.Vitality.None");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ResetViewing, "Player.Status.ResetViewing");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Equip_Axe, "Player.Status.Equip.Axe");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Attacking, "Player.Status.Attacking");
	
	
	//Set By Caller GAS中用于标记缓存数据用途。

	//** Enemy tags **//
	//Ability 技能
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SummonEnemies, "Enemy.Ability.SummonEnemies");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SpawnStone, "Enemy.Ability.SpawnStone");

	//Weapon 武器Tag，用于区分和管理不同的武器Actor。
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	//Event 事件
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SummonEnemies, "Enemy.Event.SummonEnemies");

	//Status 状态
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "Enemy.Status.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Unblockable, "Enemy.Status.Unblockable");

	//** Shared tags **//
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile");

	//Set By Caller GAS中用于标记缓存数据用途。
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Attack_BaseDamage, "Shared.SetByCaller.Attack.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Attack_BaseDamage_MultiplyCoefficient, "Shared.SetByCaller.Attack.BaseDamage.MultiplyCoefficient");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Attack_DamageIncreaseCount, "Shared.SetByCaller.Attack.DamageIncreaseCount");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Attack_DamageIncreaseCoefficient, "Shared.SetByCaller.Attack.DamageIncreaseCoefficient");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Attack_GiveTargetRage, "Shared.SetByCaller.Attack.GiveTargetRage");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Invincible, "Shared.Status.Invincible");



	//** Game Data Tags **//
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap, "GameData.Level.SurvivalGameModeMap");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_MainMenuMap, "GameData.Level.MainMenuMap");

	UE_DEFINE_GAMEPLAY_TAG(GameData_SaveGame_Slot_1, "GameData.SaveGame.Slot.1");
	
}