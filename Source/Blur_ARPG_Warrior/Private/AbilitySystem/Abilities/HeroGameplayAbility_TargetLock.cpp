// Blur Feng All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"

#include "EnhancedInputSubsystems.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controllers/WarriorHeroController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	//尝试锁定到目标。
	if (TryLockOnTarget())
	{
		InitTargetLockMovement(); //初始化锁定移动模式。
		InitTargetLockMappingContext(); //更新输入映射。
	}
	//失败时取消技能。
	else
	{
		CancelTargetLockAbility();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement(); //复位锁定移动模式。
	ResetTargetLockMappingContext(); //复位输入映射。
	CleanUp(); //清空数据。
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	// 角色死亡。
	if (UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Dead))
		return;

	// 当前锁定目标无效。
	if (!CurrentLockedActor || UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, WarriorGameplayTags::Shared_Status_Dead))
	{
		// 当目标死亡时，尝试获取一个新的目标。
		if (!SwitchTarget(true, true))
		{
			CancelTargetLockAbility();
			return;
		}
	}
  
	//更新目标锁定指示器Widget位置。
	SetTargetLockWidgetPosition();

	//确认是否重载旋转。
	const bool bShouldOverrideRotation =
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling);

	//Tips：在锁定时防御，也自动转向更符合操作直觉。
	//!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking)
	
	//更新玩家旋转面向锁定目标。
	if (bShouldOverrideRotation)
	{
		//计算看向目标需要的旋转。
		const FRotator LookAtRot =
			UKismetMathLibrary::FindLookAtRotation(
				GetHeroCharacterFromActorInfo()->GetActorLocation(),
				CurrentLockedActor->GetActorLocation())
		- FRotator(TargetLockCameraPitchOffsetDistance, 0.f, 0.f);

		//计算这帧的旋转变化值。
		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		//设置玩家控制器旋转，之后相继会跟随Controller。
		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		//设置角色旋转。
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::SwitchTarget_Triggered(const FGameplayEventData& GameplayEventData)
{
	// 获取触发时输入。
	const float InputDir = GameplayEventData.EventMagnitude;

	// 移动方向和积累方向不同，清空积累方向。
	if (InputDir > 0.f && SwitchDirectionAccumulate < 0.f
		|| InputDir < 0.f && SwitchDirectionAccumulate > 0.f)
		SwitchDirectionAccumulate = 0.f;
	
	if (bDrawDebug) Debug::Print(FString::SanitizeFloat(InputDir), FColor::Blue);
	SwitchDirectionAccumulate += InputDir;
	if (bDrawDebug) Debug::Print(FString::SanitizeFloat(SwitchDirectionAccumulate), FColor::Green);
}

void UHeroGameplayAbility_TargetLock::SwitchTarget_Completed(const FGameplayEventData& GameplayEventData)
{
	if (bDrawDebug) Debug::Print(FString::SanitizeFloat(SwitchDirectionAccumulate), FColor::Red);

	// 积累方向超过限制值。切换锁定目标。
	if (FMath::Abs(SwitchDirectionAccumulate) >= SwitchMoveDirectionLimit)
	{
		// 移动鼠标，根据移动方向切换锁定目标。
		SwitchTarget(SwitchDirectionAccumulate < 0.f);
	}
	
	SwitchDirectionAccumulate = 0.f;
}

bool UHeroGameplayAbility_TargetLock::SwitchTarget(const bool GoToLeft, const bool bStriveToGet)
{
	GetAvailableActorsToLock();

	//获取处于当前锁定目标左侧和右侧的其他有效目标。
	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	//选择距离自己最近的目标。
	if (GoToLeft)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
		if (!NewTargetToLock && bStriveToGet)
			NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
		if (!NewTargetToLock && bStriveToGet)
			NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}

	//设置当前目标为新的目标。
	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}

	return CurrentLockedActor != nullptr;
}

bool UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	//当没有有效目标时，直接取消此技能。
	if (AvailableActorsToLock.IsEmpty()) return false;

	//获取距离自身最近的目标Actor。
	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
	if (!CurrentLockedActor) return false;

	DrawTargetLockWidget(); //绘制目标锁定指示器Widget。
	SetTargetLockWidgetPosition(); //设定目标锁定Widget位置。

	return true;
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();
	TArray<FHitResult> BoxTraceHits;

	// 在Box范围内探测可锁定的目标。
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		// 获取探测盒边长总和作为最大距离平方。
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebug ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
		);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor == GetHeroCharacterFromActorInfo() || AvailableActorsToLock.Contains(HitActor)) continue;
			// 不锁定正在进场动画的敌人。
			if (UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor, WarriorGameplayTags::Enemy_Status_Entering)) continue;
			// 不锁定死亡的敌人。
			if (UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor, WarriorGameplayTags::Shared_Status_Dead)) continue;
			
			//添加敌人到有效锁定目标数组。
			AvailableActorsToLock.AddUnique(HitActor);
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	return UWarriorFunctionLibrary::GetBestTargetFromActors(
		this,
		InAvailableActors,
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetCharacterFromActorInfo()->GetActorForwardVector(),
		FMath::Square(BoxTraceDistance + TraceBoxSize.X / 2.f) + FMath::Square(TraceBoxSize.Y) + FMath::Square(TraceBoxSize.Z),
		180.f,
		false,
		false,
		SwitchTargetSelectWeight_Distance,
		SwitchTargetSelectWeight_Angle,
		bDrawDebug);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft,
	TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	//玩家位置。
	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	//玩家到当前目标向量。
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	//确认其他目标在当前目标的 左侧或右侧。
	for (AActor* Actor : AvailableActorsToLock)
	{
		if (!Actor || Actor == CurrentLockedActor) continue;

		const FVector PlayerToActorNormalized = (Actor->GetActorLocation() - PlayerLocation).GetSafeNormal();
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToActorNormalized);

		if (CrossResult.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(Actor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(Actor);
		}
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (DrawnTargetLockWidget) return;
	
	checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint."));

	//Notes：在C++中创建Widget的方式。
	
	//Notes：C2338 报错。
	//调用CreateWidget时，传参 GetHeroControllerFromActorInfo() 也需要 #include "controllers/WarriorHeroController.h"，否则编译将报错。

	//创建目标锁定指示器UI。
	DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

	check(DrawnTargetLockWidget);

	DrawnTargetLockWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	//确认UI和锁定目标有效。
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}

	//Tips：因为我们在计算屏幕位置时采用了 PlayerViewportRelative，所以在设定Widget位置时，bRemoveDPIScale设置为false。

	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true);

	//获取目标锁定Widget的尺寸。
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
	[this](UWidget* FoundWidget)
			{
				if (const USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			});
	}

	ScreenPosition -= TargetLockWidgetSize / 2.f; //修正widget位置为中心。

	//设置Widget位置。
	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	//设置角色最大移动速度。
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		//重置角色最大移动速度。
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	//获取本地玩家增强输入子系统。
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo()) return;
	
	//获取本地玩家增强输入子系统。
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	//清空锁定目标。
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	//清空Widget相关内容。
	if (DrawnTargetLockWidget)
		DrawnTargetLockWidget->RemoveFromParent();
	DrawnTargetLockWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;

	//清空缓存最大移动速度。
	CachedDefaultMaxWalkSpeed = 0.f;
}
