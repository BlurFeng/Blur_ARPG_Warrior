// Blur Feng All Rights Reserved.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"

#include "WarriorDebugHelper.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/PawnUIComponent.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	//设置属性默认值
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentVitality(1.f);
	InitMaxVitality(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

//当属性被设置时，我们在此对所有属性做最后的处理。
void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//获取人物UI接口
	if(!CachedPawnUIInterface.IsValid())
	{
		//使用弱指针缓存UI接口
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
		//CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor()); //等价方法
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface."), *Data.Target.GetAvatarActor()->GetActorNameOrLabel())

	//获取人物UI组件。
	const UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("Couldn't extract a PawnUIComponent from %s .") ,*Data.Target.GetAvatarActor()->GetActorNameOrLabel())
	
	//生命值
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		//UI更新广播
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}
	//体力值
	else if (Data.EvaluatedData.Attribute == GetCurrentVitalityAttribute())
	{
		const float NewCurrentVitality = FMath::Clamp(GetCurrentVitality(), 0.f, GetMaxVitality());
		SetCurrentVitality(NewCurrentVitality);

		//体力值达到最大。
		if (GetCurrentVitality() == GetMaxVitality())
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Vitality_Full);
		}
		//体力值变为空。
		else if (GetCurrentVitality() <= 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Vitality_None);
		}
		//移除体力值最大和体力值空Tag。
		else
		{
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Vitality_Full);
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Vitality_None);
		}
		
		//UI更新广播
		if(const UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			//只要英雄有怒气值。
			HeroUIComponent->OnCurrentVitalityChanged.Broadcast(GetCurrentVitality() / GetMaxVitality());
		}
	}
	//愤怒值
	else if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		SetCurrentRageCheck(Data, GetCurrentRage());
	}
	//伤害承受值
	else if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		//将 承受伤害值 应用到 当前生命值。
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		// const FString DebugString = FString::Printf(TEXT("Old Health: %f, Damage Done: %f, New Current Health: %f"), OldHealth, DamageDone, NewCurrentHealth);
		// Debug::Print(DebugString, FColor::Green);

		//UI更新广播
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());

		//触发死亡
		if (GetCurrentHealth() == 0.f)
		{
			//Debug::Print(TEXT("Need to Death."), FColor::Red);
			//通过给角色添加 Shared_Status_Dead 来触发 GA_Enemy_Death_Base 死亡技能。在死亡技能中，我们配置了此GA的触发方式为添加 Shared_Status_Dead Tag时。
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
		}

		//不清空此属性，作为缓存供其他计算或逻辑使用。
		//SetDamageTaken(0.f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainRageByDamageTakenCachedAttribute())
	{
		//将从承受伤害获得的怒气 应用到 当前怒气。
		const float OldRage = GetCurrentRage();
		const float GainRage = GetGainRageByDamageTakenCached();
		SetCurrentRageCheck(Data, OldRage + GainRage);
	}
}

void UWarriorAttributeSet::SetCurrentRageCheck(const struct FGameplayEffectModCallbackData& Data, float NewCurrentVal)
{
	NewCurrentVal = FMath::Clamp(NewCurrentVal, 0.f, GetMaxRage());
	SetCurrentRage(NewCurrentVal);

	//愤怒值达到最大。
	if (GetCurrentRage() == GetMaxRage())
	{
		UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
	}
	//愤怒值变为空。
	else if (GetCurrentRage() <= 0.f)
	{
		UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None);
	}
	//移除愤怒值最大和愤怒值空Tag。
	else
	{
		UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_Full);
		UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_None);
	}

	//UI更新广播
	if(const UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
	{
		//只要英雄有怒气值。
		HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
	}
}
