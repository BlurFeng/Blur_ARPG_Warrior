// Blur Feng All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_GainRageByDaTa.h"

#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"

// 在这个结构体中，我们定义并获取计算需要用到的属性。
struct FWarriorGainRageByDamageTakenCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	DECLARE_ATTRIBUTE_CAPTUREDEF(GainRageByDamageTakenCoefficient)
	DECLARE_ATTRIBUTE_CAPTUREDEF(GainRageByDamageTakenCached)
	
	FWarriorGainRageByDamageTakenCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, GainRageByDamageTakenCoefficient, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, GainRageByDamageTakenCached, Target, false);
	}
};
// 一个全局静态结构体
static const FWarriorGainRageByDamageTakenCapture& GetWarriorGainRageByDamageTakenCapture()
{
	static FWarriorGainRageByDamageTakenCapture WarriorGainRageByDamageTakenCapture;
	return WarriorGainRageByDamageTakenCapture;
}

UGEExecCalc_GainRageByDaTa::UGEExecCalc_GainRageByDaTa()
{
	//捕获属性列表。在Editor中还可以根据需求进一步对这些参数进行修饰。
	RelevantAttributesToCapture.Add(GetWarriorGainRageByDamageTakenCapture().DamageTakenDef);
	RelevantAttributesToCapture.Add(GetWarriorGainRageByDamageTakenCapture().GainRageByDamageTakenCoefficientDef);
	RelevantAttributesToCapture.Add(GetWarriorGainRageByDamageTakenCapture().GainRageByDamageTakenCachedDef);
}

void UGEExecCalc_GainRageByDaTa::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();

	//准备参数。
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	//目标承受的伤害。
	float TargetDamageTaken = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorGainRageByDamageTakenCapture().DamageTakenDef, EvaluateParameters, TargetDamageTaken);
	// Debug::Print(TEXT("TargetDamageTaken"), TargetDamageTaken);

	//目标从承受伤害获得怒气的系数。
	float TargetGainRageByDamageTakenCoefficient = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorGainRageByDamageTakenCapture().GainRageByDamageTakenCoefficientDef, EvaluateParameters, TargetGainRageByDamageTakenCoefficient);
	// Debug::Print(TEXT("TargetGainRageByDamageTakenCoefficient"), TargetGainRageByDamageTakenCoefficient);

	//计算获得的怒气值。
	float FinalGainRage = TargetDamageTaken * TargetGainRageByDamageTakenCoefficient;

	if (FinalGainRage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorGainRageByDamageTakenCapture().GainRageByDamageTakenCachedProperty,
				EGameplayModOp::Override,
				FinalGainRage
			)
		);
	}
}
