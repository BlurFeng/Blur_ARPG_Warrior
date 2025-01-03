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

private:
	//尝试锁定目标。
	void TryLockOnTarget();

	//获取何用的锁定目标。
	void GetAvailableActorsToLock();

	//获取距离自身最近的目标。
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	//绘制目标锁定指示器UI。
	void DrawTargetLockWidget();

	//设置目标锁定指示器UI位置。
	void SetTargetLockWidgetPosition();

	//取消技能自身。
	void CancelTargetLockAbility();

	//清除数据。
	void CleanUp();

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
	bool bDrawDebugForBoxTrace;

	//锁定目标指示器Widget。
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;

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
};