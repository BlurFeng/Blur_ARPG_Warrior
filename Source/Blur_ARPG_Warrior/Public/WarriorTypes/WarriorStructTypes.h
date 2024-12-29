// Blur Feng All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

//Notes：generated.h
//在UE中，我们必须需要 #include "WarriorStructTypes.generated.h" 才能成功编译这个文件。
//每个包含 GENERATED_BODY() 或类似宏（如 GENERATED_UCLASS_BODY）的类或结构，必须在对应的头文件中包含一个 .generated.h 文件。
//编译顺序依赖： Unreal Engine 的代码生成是一个多阶段的编译过程，generated.h 文件的内容在普通编译器处理之前由 UHT 生成，缺少它会导致 GENERATED_BODY() 无法展开。
//包含 #include "WarriorStructTypes.generated.h" 是使用 Unreal Engine 特定功能（如反射、蓝图支持、序列化）的必需步骤。
//确保 .generated.h 文件正确生成和引用是 USTRUCT、UCLASS 和 UENUM 正常运行的核心前提。

#include "WarriorStructTypes.generated.h"

class UInputMappingContext;
class UWarriorHeroLinkedAnimLayer;
class UWarriorGameplayAbility;

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
	TSubclassOf<UWarriorGameplayAbility> AbilityToGrant;

	bool IsValid() const;
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

	//武器默认技能组。在装备武器时赋予角色。比如装备斧子时获得“卸下斧子”、“攻击”、“防御”等技能。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroAbilitySet> DefaultWeaponAbilities;

	//Tips：实际上我们的EquipAxe技能也不应当直接赋予角色，而是在角色获得Axe时赋予。
	//但对于这个学习项目，这样就好。否则我们还需要开发相应的道具系统和拾取道具的功能。
};