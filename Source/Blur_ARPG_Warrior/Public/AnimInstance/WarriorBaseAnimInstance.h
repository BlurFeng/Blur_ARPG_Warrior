// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"

#include "WarriorBaseAnimInstance.generated.h"

//动画实例父类
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	/// 确认所有者拥有某个Tag。
	/// @param TagToCheck 
	/// @return 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
};
