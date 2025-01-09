// Blur Feng All Rights Reserved.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

// 在这个结构体中，我们定义并获取计算需要用到的属性。
struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	
	FWarriorDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false);//从caster施法者获取攻击力
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false);//从target目标获取防御力
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false);
	}
};
// 一个全局静态结构体
static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	// 通过 DECLARE_ATTRIBUTE_CAPTUREDEF 宏省略下面的代码
	// //找到需要的属性
	// FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	// 	UWarriorAttributeSet::StaticClass(),
	// 	GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, AttackPower)
	// 	);
	// //获取需要的属性。EGameplayEffectAttributeCaptureSource决定从caster施法者身上获取属性，还是从目标身上获取属性。
	// //比如进行攻击时，我们从施法者获取攻击力属性，从目标获取防御属性。然后计算伤害。
	// FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	// 	AttackPowerProperty,
	// 	EGameplayEffectAttributeCaptureSource::Source,
	// 	false
	// 	);
	// //添加计算需要用到的属性到RelevantAttributesToCapture数组
	// RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	//捕获属性列表。在Editor中还可以根据需求进一步对这些参数进行修饰。
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();
	
	// 上下文 Context 信息，可以根据需要使用下列信息。
	// 是在UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle()方法中传入的。
	// EffectSpec.GetContext().GetSourceObject();
	// EffectSpec.GetContext().GetAbility();
	// EffectSpec.GetContext().GetInstigator();
	// EffectSpec.GetContext().GetEffectCauser();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 获取caster施法者攻击力
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	// Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	//获取目标防御力
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	// Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);
	
	// Tips：SetByCallerTagMagnitudes 缓存到Data中的值用于计算。搜索 SetByCallerTagMagnitudes 找到使用处。
	float BaseDamage = 0.f; //基础伤害，取决于使用的武器。
	float BaseDamageMultiplyCoefficient = 1.f; //基础伤害乘系数。
	int32 DamageIncreaseCount = 0; //增伤计数，一般和连击段数相关。
	float DamageIncreaseCoefficient = 0.f; //增伤系数。
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_Attack_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			// Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}
		else if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_Attack_BaseDamage_MultiplyCoefficient))
		{
			BaseDamageMultiplyCoefficient = TagMagnitude.Value;
			// Debug::Print(TEXT("BaseDamageMultiplyCoefficient"), BaseDamageMultiplyCoefficient);
		}
		else if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_Attack_DamageIncreaseCount))
		{
			DamageIncreaseCount = TagMagnitude.Value;
			// Debug::Print(TEXT("DamageIncreaseCount"), DamageIncreaseCount);
		}
		else if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_Attack_DamageIncreaseCoefficient))
		{
			DamageIncreaseCoefficient = TagMagnitude.Value;
			// Debug::Print(TEXT("DamageIncreaseCoefficient"), DamageIncreaseCoefficient);
		}
	}

	// 基础伤害乘系数。
	BaseDamage *= BaseDamageMultiplyCoefficient;
	
	// 根据计数和系数增加基础伤害。
	const float DamageIncreasePercent = DamageIncreaseCount * DamageIncreaseCoefficient + 1.f;
	BaseDamage *= DamageIncreasePercent;

	// 最终伤害。根据施法者攻击力和被害者防御力计算。
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	// // Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
