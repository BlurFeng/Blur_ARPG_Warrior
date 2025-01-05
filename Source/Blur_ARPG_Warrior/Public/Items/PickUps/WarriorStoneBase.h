// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/WarriorPickUpBase.h"
#include "WarriorStoneBase.generated.h"

class UGameplayEffect;
class UWarriorAbilitySystemComponent;
//石头道具基类。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorStoneBase : public AWarriorPickUpBase
{
	GENERATED_BODY()

public:
	void Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//当石头消耗时。我们可以在蓝图中处理音效和特效的逻辑。蓝图子类还需要自己调用DestroyActor()来销毁自身。
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Stone Consumed")
	void BP_OnStoneConsumed();
	
	//石头效果GE。
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
