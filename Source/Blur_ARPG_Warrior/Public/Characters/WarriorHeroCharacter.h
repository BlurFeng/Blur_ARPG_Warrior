// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorHeroCharacter.generated.h"

//Notes:
//Forward Declaration 前向声明，告知编译器这是类的名称而无需在当前文件中提供完整定义。
//减少编译依赖：直接#include来引入完整的头文件时，本文件的任何修改都会导致相关文件重新编译。
//我们只在.cpp中实际用到类的具体功能时才#include他们的头文件。
class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;

//英雄角色类，玩家控制的角色。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorHeroCharacter();

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

private:
#pragma region Components

	//Notes:
	//UPROPERTY宏用来标记UE类中的成员变量，告知引擎如何管理这些成员变量。
	//VisibleAnywhere：在UE编辑器的任何地方可见。只读。
	//BlueprintReadOnly：蓝图中只读。如果想要编辑使用BlueprintReadWrite。
	//Category：分类，有助于整理代码。蓝图中也会显示到相应分类下。
	//meta = (AllowPrivateAccess = "true")：允许蓝图或编辑器访问private成员变量。

	//相机臂
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//自动跟随的相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
#pragma endregion

#pragma region Inputs
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	
#pragma endregion

	
};
