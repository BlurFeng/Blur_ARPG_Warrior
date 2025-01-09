// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "GEExecCalc_GainRageByDaTa.generated.h"

//用于计算在承受上海时获得的怒气。
UCLASS()
class BLUR_ARPG_WARRIOR_API UGEExecCalc_GainRageByDaTa : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEExecCalc_GainRageByDaTa();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
