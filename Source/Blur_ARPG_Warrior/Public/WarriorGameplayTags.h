// Blur Feng All Rights Reserved.

#pragma once

//Notes:
//我们通过C++来创建GameplayTags。而不是通过UE的主菜单>Project Settings>GameplayTags>Gameplay Tag List的Manage Gameplay Tags编辑器工具进行创建。
//这样可以在C++中更方便的访问GameplayTags而不用担心拼写错误。
//UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")定义Tag时，InputTag_Move是用于C++访问的名称。InputTag.Move是显示名称。

#include "NativeGameplayTags.h"

namespace WarriorGameplayTags
{
	//Notes：BLUR_ARPG_WARRIOR_API指定了我们可以使用这个宏的项目API，根据项目名称不同这个API也不同
	
	//** Input Tags **//
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);

	//** Player tags **//
	//Ability 技能
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);

	//Weapon 武器Tag，用于区分和管理不同的武器Actor
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	//Event 事件
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);

	//Status 状态
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

	
	//** Enemy tags **//
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);
}