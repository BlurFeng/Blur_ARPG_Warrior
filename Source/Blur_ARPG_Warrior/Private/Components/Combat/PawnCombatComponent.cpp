// Blur Feng All Rights Reserved.

#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
                                                 AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponsMap.Contains(InWeaponTagToRegister), TEXT("A named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	CharacterCarriedWeaponsMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	//绑定回调方法到武器委托
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &UPawnCombatComponent::OnWeaponPulledFromTargetActor);

	//当生成武器时，如果武器直接进行装备时，直接设置CurrentEquippedWeaponTag。
	//否则一般通过“装备武器”技能，并在Ability蓝图中修改。
	if(bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

	//const FString WeaponString =  FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	//Debug::Print(WeaponString);
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	//Notes：双重指针。
	//TMap存储的实际上是指向常量指针的指针，我们通过解指针获得需要的指向常量的指针并作为返回值。
	//const*的作用是确保通过双重指针无法直接修改最终指向的常量。
	//AActor* 普通指针； AActor* const 常量指针，指向地址不可修改； const AActor* 只读指针，内容不可修改；
	//const AActor* const 完全常量指针，地址和内容都不可更改； AActor* const* 指向常量指针的指针；
	//const AActor* const* 指向 指向只读内容的常量指针 的指针 

	if(CharacterCarriedWeaponsMap.Contains(InWeaponTagToGet))
	{
		if(AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponsMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if(!CurrentEquippedWeaponTag.IsValid()) return nullptr;
	
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if(ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

		check(WeaponToToggle);

		//开启或关闭武器碰撞盒检测
		if (bShouldEnable)
		{
			//Debug::Print(WeaponToToggle->GetName() + TEXT("Collision enabled."), FColor::Green);
			//QueryOnly 仅查询。能够检测到碰撞但碰撞盒没有实际的物理体积。
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			//Debug::Print(WeaponToToggle->GetName() + TEXT("Collision disabled."), FColor::Red);
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//在关闭碰撞盒时，清空造成过效果的目标Actors缓存列表。
			OverlappedActors.Empty();
		}
	}

	//TODO：Handle body collision boxes
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
