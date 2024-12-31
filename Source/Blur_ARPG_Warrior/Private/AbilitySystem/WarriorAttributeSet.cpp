// Blur Feng All Rights Reserved.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	//设置属性默认值
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//当属性被设置时，我们在此限制最终实际的值。

	//生命值
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
	}

	//愤怒值
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
	}

	//伤害承受值
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		//将 承受伤害值 应用到 当前生命值。
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		const FString DebugString = FString::Printf(TEXT("Old Health: %f, Damage Done: %f, New Current Health: %f"), OldHealth, DamageDone, NewCurrentHealth);
		Debug::Print(DebugString, FColor::Green);
		
		//TODO: Notify the UI

		//触发死亡
		if (NewCurrentHealth == 0.f)
		{
			//Debug::Print(TEXT("Need to Death."), FColor::Red);
			//通过给角色添加 Shared_Status_Dead 来触发 GA_Enemy_Death_Base 死亡技能。在死亡技能中，我们配置了此GA的触发方式为添加 Shared_Status_Dead Tag时。
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
			
		}
	}
}
