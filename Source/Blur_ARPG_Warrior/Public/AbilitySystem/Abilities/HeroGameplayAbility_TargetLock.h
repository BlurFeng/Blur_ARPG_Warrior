// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Widgets/WarriorWidgetBase.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
//锁定目标技能。
//技能逻辑较为复杂，我们在C++中实现其核心功能。
UCLASS()
class BLUR_ARPG_WARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget_Triggered(const FGameplayEventData& GameplayEventData);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget_Completed(const FGameplayEventData& GameplayEventData);

	UFUNCTION()
	bool SwitchTarget(const bool GoToLeft, const bool bStriveToGet = false);

private:
	//尝试锁定目标。
	bool TryLockOnTarget();
	//获取何用的锁定目标。
	void GetAvailableActorsToLock();
	//获取距离自身最近的目标。
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	//在当前目标周围找寻可用的目标。
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);

	//绘制目标锁定指示器UI。
	void DrawTargetLockWidget();
	//设置目标锁定指示器UI位置。
	void SetTargetLockWidgetPosition();

	//初始化锁定目标移动模式。
	void InitTargetLockMovement();
	//复位锁定目标移动模式。
	void ResetTargetLockMovement();

	//初始化锁定目标输入映射。
	void InitTargetLockMappingContext();
	//复位锁定目标输入映射。
	void ResetTargetLockMappingContext();

	//取消技能自身。
	void CancelTargetLockAbility();
	//清除数据。
	void CleanUp();

	
	//~ 配置
	//锁定目标探测盒距离。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	float BoxTraceDistance = 5000.f;

	//锁定目标探测盒尺寸。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	//碰撞通道。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	//绘制BoxTrace的Debug信息。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	bool bDrawDebug;

	//锁定目标指示器Widget。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

	// 切换目标选择时，距离权重。权重越高，越会选择距离自己近的目标。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock", meta = (ClampMin = 1))
	int SwitchTargetSelectWeight_Distance = 1;

	// 切换目标选择时，角度权重。权重越高，越会选择和自己正面方向角度接近的目标。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock", meta = (ClampMin = 1))
	int SwitchTargetSelectWeight_Angle = 1;

	//旋转速度。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;

	//开启目标锁定时的最大移动速度。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	float TargetLockMaxWalkSpeed = 220.f;

	//锁定目标时的操作映射。覆盖原有操作映射。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	class UInputMappingContext* TargetLockMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	float TargetLockCameraOffsetDistance = 20.f;


	//~ 缓存数据
	//可用于锁定的目标Actors数组缓存。
	TArray<AActor*> AvailableActorsToLock;

	//当前锁定的目标。
	UPROPERTY()
	AActor* CurrentLockedActor;

	//目标锁定指示器Widget。
	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetLockWidget;

	//目标锁定指示器Widget尺寸。用于修正Widget的最终位置。
	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	//进入目标锁定状态时的当前最大移动速度。
	UPROPERTY()
	float CachedDefaultMaxWalkSpeed;

	// 切换目标时需要移动的距离量。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	float SwitchMoveDirectionLimit = 10.f;
	
	UPROPERTY()
	float SwitchDirectionAccumulate;
};
