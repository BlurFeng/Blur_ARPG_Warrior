// Blur Feng All Rights Reserved.


#include "AbilitySystem/WarriorAttributeSet.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	//设置属性默认值
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}
