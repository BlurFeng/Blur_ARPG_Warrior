// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "WarriorAttributeSet.generated.h"

//我们使用宏帮我们快速定义一个属性对应的初始化、设置、获取方法。
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;

//基本属性集
//在 AWarriorBaseCharacter::AWarriorBaseCharacter() 中创建并附加给角色。
UCLASS()
class BLUR_ARPG_WARRIOR_API UWarriorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWarriorAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	//生命
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxHealth)

	//体力
	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	FGameplayAttributeData CurrentVitality;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentVitality)

	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	FGameplayAttributeData MaxVitality;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxVitality)

	//怒气
	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentRage)

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxRage)

	//攻击力
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, AttackPower)

	//防御力
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DefensePower)

	//伤害接收数值缓存。
	//用于暂时存储受到的伤害，最终作用于CurrentHealth。方便我们在结算伤害前再进行其他计算。
	//Tips：此值是Override的并不累加。理论上来说一帧之内不会有多个伤害计算 override DamageTaken的值。如果出问题需要扩展修改相关逻辑。
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DamageTaken)

	//从受到伤害获取的怒气值系数。获得怒气值 = DamageTaken * GainRageByDamageTakenCoefficient。
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData GainRageByDamageTakenCoefficient;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, GainRageByDamageTakenCoefficient)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData GainRageByDamageTakenCached;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, GainRageByDamageTakenCached)

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;

	void SetCurrentRageCheck(const struct FGameplayEffectModCallbackData& Data, float NewCurrentVal);
};
