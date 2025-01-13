// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorSaveGame.generated.h"

//游戏存档数据。
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//游戏难度。
	UPROPERTY(BlueprintReadOnly)
	EWarriorGameDifficulty SavedGameDifficulty = EWarriorGameDifficulty::Normal;
};
