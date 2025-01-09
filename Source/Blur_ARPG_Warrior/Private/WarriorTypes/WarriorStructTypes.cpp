// Blur Feng All Rights Reserved.


#include "WarriorTypes/WarriorStructTypes.h"

#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

#if WITH_EDITOR
void FWarriorHeroAbilitySet::PostEditChangeProperty()
{
	//成员属性变化时被调用，可在此处按需求调整或限制字段。
}
#endif
