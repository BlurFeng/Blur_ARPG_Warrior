// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"

#include "WarriorBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UWarriorAttributeSet;
class UWarriorAbilitySystemComponent;
//角色基础类型，之后派生子类Hero或者Enemy等。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarriorBaseCharacter();

	//Notes：IAbilitySystemInterface 接口是GAS和Character通信的重要接口。
	//GAS通过此接口和Character通信。此接口在GAS系统的各种地方使用到。

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	//技能系统组件（属于GAS插件的一部分）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AbilitySystem")
	UWarriorAbilitySystemComponent* WarriorAbilitySystemComponent;

	//属性集（属于GAS插件的一部分）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AbilitySystem")
	UWarriorAttributeSet* WarriorAttributeSet;

	//角色启动数据资源
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:

	//Notes：FORCEINLINE宏用于强制内联化函数。
	//非常适合用于访问器方法这样频繁调用且短小的方法，以提高性能。
	//它将函数的实现直接嵌入调用点，而不是生成函数调用指令，通过栈跳转来调用函数。
	//但需要注意，这会导致代码膨胀，不适合用于较大的函数。否则会增加缓存压力，适得其反。
	
	FORCEINLINE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }
	FORCEINLINE UWarriorAttributeSet* GetWarriorAttributeSet() const { return WarriorAttributeSet; }
};
