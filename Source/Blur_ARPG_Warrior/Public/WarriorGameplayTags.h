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

	//** Player tags **//
	BLUR_ARPG_WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);
}