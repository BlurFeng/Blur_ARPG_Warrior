// Blur Feng All Rights Reserved.


#include "WarriorGameInstance.h"

TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(const FGameplayTag InTag) const
{
	for (const FWarriorGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid()) continue;

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
