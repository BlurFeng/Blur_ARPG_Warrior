// Blur Feng All Rights Reserved.

#include "Characters/WarriorHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorInputComponent.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Components/Combat/HeroCombatComponent.h"

#include "WarriorDebugHelper.h"

//初始化。
AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	//关闭Rotation由控制器控制。
	bUseControllerRotationPitch = false; //俯仰角。
	bUseControllerRotationYaw = false; //偏航角。
	bUseControllerRotationRoll = false; //翻滚角。

	//创建相机臂。
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //创建子对象，这个对象将包含在自身之下。
	CameraBoom->SetupAttachment(GetRootComponent()); //附着目标为角色自身的Root。
	CameraBoom->TargetArmLength = 200.f; //目标相机臂长度。
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f); //相机位置偏移，肩视角。
	CameraBoom->bUsePawnControlRotation = true; //使用Pawn的旋转作为相机旋转。

	//创建相机。
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //附着目标为相机臂。
	FollowCamera->bUsePawnControlRotation = true;

	//设置角色移动组件。
	GetCharacterMovement()->bOrientRotationToMovement = true; //旋转角色自身到加速度方向。
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); //旋转速率。
	GetCharacterMovement()->MaxWalkSpeed = 400.f; //最大移动速度。
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; //刹车减速度。

	//创建战斗组件
	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
}

UPawnCombatComponent* AWarriorHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if(WarriorAbilitySystemComponent && WarriorAttributeSet)
	// {
	// 	const FString ASCText =
	// 		FString::Printf(TEXT("Owner Actor: %s, Avatar Actor: %s"),
	// 			*WarriorAbilitySystemComponent->GetOwnerActor()->GetActorLabel(),
	// 			*WarriorAbilitySystemComponent->GetAvatarActor()->GetActorLabel());
	// 	Debug::Print(TEXT("Ability system component valid. ") + ASCText, FColor::Green);
	// 	Debug::Print(TEXT("Attribute Set valid. ") + ASCText, FColor::Green);
	// }

	if(!CharacterStartUpData.IsNull())
	{
		//Notes：同步加载和异步加载。
		//因为CharacterStartUpData中的内容我们希望在一开始就加载好，所以使用Synchronous Loading同步加载。
		//如果不是立刻使用，或者需要大量时间加载的，建议使用Asynchronous Loading异步加载。比如敌人，场景等。
		//这里是同步加载的范例，异步加载的范例可以查看加载敌人相关资源时的代码。
		
		if(UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
		}
	}
}

//Notes：Enhanced Input增强输入系统
//基于UE5新的Enhanced Input增强输入系统，我们可以在ProjectSettings>Engine>Input中配置相关资源。
//我们设置了项目的Default Input Component Class为WarriorInputComponent类。

//此重载方法用于设置玩家输入组件。
void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//确认InputConfigDataAsset资源是否配置，不为null。
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config."));

	//调用父类方法，实际上里面没有具体的内容，但之后引擎更新可能会有实际实现的内容。
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//获取本地玩家控制器。
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	//通过增强输入子系统来添加配置的输入映射文件。
	//在输入映射文件中，我们给每个InputAction配置可以出发的硬件输入，比如键盘或手柄的输入。
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	//绑定输入事件。
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent); //将输入组件转换为UWarriorInputComponent类
	WarriorInputComponent->BindNativeInputAction(
		InputConfigDataAsset, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	WarriorInputComponent->BindNativeInputAction(
		InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Debug::Print(TEXT("WarriorHeroCharacter BeginPlay."));
}

void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>(); //获取输入值

	//移动方向旋转数据，根据玩家控制器Yaw旋转构建。
	//之后通过此Rotator构建向先后和左右移动的方向。实际上Control的朝向和Camera一致，我们就能根据Camera朝向进行移动。
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	//前后移动
	if(MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector); //构建移动方向
		AddMovementInput(ForwardDirection, MovementVector.Y); //按轴向进行移动
	}

	//左右移动
	if(MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	//左右摆动
	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	//上下摆动
	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
