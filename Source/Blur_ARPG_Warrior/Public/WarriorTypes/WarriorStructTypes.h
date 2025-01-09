// Blur Feng All Rights Reserved.

#pragma once

//Notes：Error C2665 : “FGameplayAbilitySpec::FGameplayAbilitySpec”: 没有重载函数可以转换所有参数类型。
//使用 #include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h" 完整引用 代替 class UWarriorHeroGameplayAbility 前置引用来解决这个问题。
//否则在其他类中引用 WarriorStructTypes.h 并使用其中的数据结构，并用到这些 前置引用的类 时可能出现问题。
//比如一个方法要求传入 class A，但你 传入了他的子类 class A1，但 A1 是前置引用的，所以方法并不知道 A1 是 A 的子类，导致方法无法使用。如果他是重载方法，就会报错没找到可用的重载方法。
//为了安全起见，我们总是为我们自己创造的 子类 进行#include完整引用。
//一些UE引擎原生的类，我们依然可以使用 前置引用。或者这个类在蓝图中使用时，并不会有相关的报错产生。
//还有一点需要注意，我们使用 增量编译 时，这样的问题可能被隐藏，知道下一次被编译（修改内容或完整重编译）时才会暴露此问题。

#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

//Notes：generated.h
//在UE中，我们必须需要 #include "WarriorStructTypes.generated.h" 才能成功编译这个文件。
//每个包含 GENERATED_BODY() 或类似宏（如 GENERATED_UCLASS_BODY）的类或结构，必须在对应的头文件中包含一个 .generated.h 文件。
//编译顺序依赖： Unreal Engine 的代码生成是一个多阶段的编译过程，generated.h 文件的内容在普通编译器处理之前由 UHT 生成，缺少它会导致 GENERATED_BODY() 无法展开。
//包含 #include "WarriorStructTypes.generated.h" 是使用 Unreal Engine 特定功能（如反射、蓝图支持、序列化）的必需步骤。
//确保 .generated.h 文件正确生成和引用是 USTRUCT、UCLASS 和 UENUM 正常运行的核心前提。

#include "WarriorStructTypes.generated.h"

class UInputMappingContext;
class UWarriorHeroLinkedAnimLayer;

//英雄技能设置数据
USTRUCT(BlueprintType)
struct FWarriorHeroAbilitySet
{
	GENERATED_BODY()

	//输入Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	//赋予技能组
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

//特殊技能配置。
USTRUCT(BlueprintType)
struct FWarriorHeroSpecialAbilitySet : public FWarriorHeroAbilitySet
{
	GENERATED_BODY()

	//技能Icon材质。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> SoftAbilityIconMaterial;

	//技能冷却Tag。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Player.Cooldown"))
	FGameplayTag AbilityCooldownTag;
};

//英雄武器数据
USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	//武器的链接动画层。在装备武器时Link到角色Mesh并作用于动画蓝图来更改角色动画表现。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroLinkedAnimLayer> WeaponAnimLayerToLink;

	//武器输入映射。当装备武器时，使用此InputMap覆盖原有的。因为装备武器后操作方式会相应变化。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	//Tips：实际上我们的EquipAxe技能也不应当直接赋予角色，而是在角色获得Axe时赋予。
	//但对于这个学习项目，这样就好。否则我们还需要开发相应的道具系统和拾取道具的功能。

	//武器默认技能组。在装备武器时赋予角色。比如装备斧子时获得“卸下斧子”、“攻击”、“防御”等技能。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroAbilitySet> DefaultWeaponAbilities;

	//特殊武器技能。强力的技能，有CD。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroSpecialAbilitySet> SpecialWeaponAbilities;

	//使用曲线设置武器的基础伤害。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	//武器图标Icon。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};