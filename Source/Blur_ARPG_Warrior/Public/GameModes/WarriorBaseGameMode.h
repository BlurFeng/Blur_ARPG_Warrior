// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorTypes/WarriorEnumTypes.h"

#include "WarriorBaseGameMode.generated.h"

//游戏模式基类。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWarriorBaseGameMode();

protected:

	//当前游戏难度。
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Settings")
	EWarriorGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EWarriorGameDifficulty GetCurrentGameDifficulty() const {return CurrentGameDifficulty;}
};
