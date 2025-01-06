// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "GameplayEffectTypes.h"

#include "WarriorFunctionLibrary.generated.h"

class UWarriorGameInstance;
class UPawnCombatComponent;
struct FGameplayTag;
class UWarriorAbilitySystemComponent;
struct FScalableFloat;

//项目方法库
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// 获取AbilitySystemComponent从Actor。
	/// @param InActor 
	/// @return 
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	/// 向Actor（的技能组件）添加一个还未包含的Tag。
	/// @param InActor 目标Actor。
	/// @param TagToAdd 添加Tag。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/// 移除一个Tag从Actor。
	/// @param InActor 目标Actor。
	/// @param TagToRemove 移除Tag。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	/// 确认Actor（的技能组件）是否包含Tag。
	/// @param InActor 目标Actor。
	/// @param TagToCheck 确认Tag。
	/// @return 
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	//Notes：ExpandEnumAsExecs 执行引脚
	//meta = (ExpandEnumAsExecs = "OutConfirmType") 将参数 OutConfirmType 枚举类型转化为蓝图中的执行引脚。免去了Branch判断，便于使用。
	//Notes：DisplayName 显示名称
	//在C++中根据用途，我们可能为某一功能创建一系列的方法，采用不同前缀的命名方式。但在蓝图中，我们不希望显示这些前缀。
	//可以通过 meta = (DisplayName = "Does Actor Have Tag") 来修改显示名称。

	/// 确认Actor是否包含某个Tag。
	/// @param InActor 目标Actor。
	/// @param TagToCheck 确认Tag。
	/// @param OutConfirmType 返回确认结果。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);

	/// 获取人物战斗组件，从Actor。
	/// @param InActor 
	/// @return 
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	/// 获取人物战斗组件，从Actor。
	/// @param InActor 
	/// @param OutValidType 获取组件是否有效。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	/// 确认目标Pawn是否是敌对的。
	/// @param QueryPawn 
	/// @param TargetPawn 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn);

	//Notes：CompactNodeTitle
	//使用 meta = (CompactNodeTitle = "Get Value At Level") 让此方法在蓝图中的节点显示为紧凑节点和输入的文字。
	
	/// 从ScalableFloat获取Float，根据Level。
	/// @param InScalableFloat 
	/// @param InLevel 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel);

	/// 计算收击方向。
	/// @param InAttacker 攻击者。
	/// @param InVictim 受害者。
	/// @param OutAngleDifference 角度差。 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim, float& OutAngleDifference);

	/// 确认是否是有效的格挡。
	/// @param InAttacker 
	/// @param InDefender 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	/// 将GE应用到目标Actor上。
	/// @param InInstigator 施法者。
	/// @param InTargetActor 目标Actor。
	/// @param InSpecHandle GE说明句柄。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, FGameplayEffectSpecHandle& InSpecHandle);

	//Notes：Latent Action 潜在事件节点。
	//通过 Latent Action 我们可以实现一些蓝图的异步节点。比如常用的 Delay 就是一个 Latent Action。
	//Notes：元数据说明符。
	//Latent：标记此方法为 Latent 方法。
	//LatentInfo：此方法必须有 FLatentActionInfo 参数并通过 LatentInfo 标记。
	//WorldContext：自动获得 WorldContextObject 参数。当我们的方法需要 世界 信息时，可以通过此方式自动获取。在生成Actor，计时器，全局设置等时候。
	//UPARAM(DisplayName = "Output")：我们可以在参数前使用UPARAM()进行说明。这里重设了显示名称以防止过长的文字。

	/// 倒计时异步方法。
	/// @param WorldContextObject 
	/// @param TotalTime 总时间。
	/// @param UpdateInterval 更新间隔。
	/// @param ExecuteOnFirst 在开始的第一帧执行。
	/// @param OutRemainingTime 剩余时间。
	/// @param CountDownInput 输入执行引脚。
	/// @param CountDownOutput 输出执行引脚。
	/// @param LatentInfo 
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1", ExecuteOnFirst = "true"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, bool ExecuteOnFirst,
		float& OutRemainingTime, EWarriorCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput,
		FLatentActionInfo LatentInfo);

	/// 获取游戏实例。
	/// @param WorldContextObject 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UWarriorGameInstance* GetWarriorGameInstance(const UObject* WorldContextObject);

	/// 设置输入模式。在游戏模式和UI模式中切换。
	/// @param WorldContextObject 
	/// @param InInputMode 
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void SetInputMode(const UObject* WorldContextObject, const EWarriorInputMode InInputMode);

	/// 存储游戏难度设置。
	/// @param InDifficultyToSave 
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void SaveCurrentGameDifficulty(EWarriorGameDifficulty InDifficultyToSave);

	/// 读取存档的游戏难度。
	/// @param OutSavedDifficulty 
	/// @return 是否成功读取。及时失败，我们也会返回一个默认的 OutSavedDifficulty。
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static bool TryLoadSavedGameDifficulty(EWarriorGameDifficulty& OutSavedDifficulty);

	/// 获取Enum对应的String。
	/// @param EnumValue 枚举值。
	/// @return 
	template <class TEnum>
	static FString GetEnumString(TEnum EnumValue);
};

template <class TEnum>
FString UWarriorFunctionLibrary::GetEnumString(TEnum EnumValue)
{
	const UEnum* EnumPtr = StaticEnum<TEnum>();
	if (!EnumPtr) return "Invalid";
	return EnumPtr->GetNameStringByValue(static_cast<int32>(EnumValue));
}
