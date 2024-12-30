// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "WarriorTypes/WarriorStructTypes.h"

#include "DataAsset_HeroStartUp.generated.h"

//英雄启动数据资源
UCLASS()
class BLUR_ARPG_WARRIOR_API UDataAsset_HeroStartUp : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	//英雄启动时技能设置组。用于在启动时赋予角色技能，这些技能是主动或被动技能，在必要时激活。他们都是游玩时使用到的技能，有复杂的状态机制。
	UPROPERTY(EditDefaultsOnly, category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroAbilitySet> HeroStartUpAbilitySets;
};
