// Blur Feng All Rights Reserved.


#include "Controllers/WarriorHeroController.h"

AWarriorHeroController::AWarriorHeroController()
{
	//设置英雄队伍Id。用于敌人AI系统。
	HeroTeamId = FGenericTeamId(0);
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
