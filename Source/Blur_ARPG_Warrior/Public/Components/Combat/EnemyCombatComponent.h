// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

//敌人战斗组件。
UCLASS()
class BLUR_ARPG_WARRIOR_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	//当击中目标时。
	virtual void OnHitTargetActor(AActor* HitActor) override;

protected:
	virtual void ToggleBodyCollisionBoxCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType) override;
};
