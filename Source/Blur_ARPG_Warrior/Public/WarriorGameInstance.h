// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "WarriorGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FWarriorGameLevelSet
{
	GENERATED_BODY()

	//关卡Tag。
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	//关卡。
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

//游戏实例。可用于全局数据管理。
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	//加载地图前。
	virtual void OnPreloadMap(const FString& MapName);
	//目标地图加载完成后。
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);

	//游戏地图设置。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWarriorGameLevelSet> GameLevelSets;

public:
	
	/// 获取游戏关卡，根据Tag。
	/// @param InTag 
	/// @return 
	UFUNCTION(BlueprintPure, meta = (Categories = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(const FGameplayTag InTag) const;
};
