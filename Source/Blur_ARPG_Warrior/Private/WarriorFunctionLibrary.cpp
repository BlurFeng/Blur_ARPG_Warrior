// Blur Feng All Rights Reserved.


#include "WarriorFunctionLibrary.h"

#include "WarriorDebugHelper.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/WarriorSaveGame.h"
#include "Common/WarriorCountDownAction.h"

#pragma region Tools

void UWarriorFunctionLibrary::CountDown(
	const UObject* WorldContextObject, float TotalTime, float UpdateInterval, bool ExecuteOnFirst,
	float& OutRemainingTime, EWarriorCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput,
	FLatentActionInfo LatentInfo)
{
	//确认World可用。
	UWorld* World = nullptr;
	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	if (!World) return;

	//查找LatentAction。
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	FWarriorCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FWarriorCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	//输入执行为开始。
	if (CountDownInput == EWarriorCountDownActionInput::Start)
	{
		//创建LatentAction。
		//此处 new 的类将由 LatentActionManager 进行管理，我们无需担心内存泄露问题。
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID, new FWarriorCountDownAction(TotalTime, UpdateInterval, ExecuteOnFirst, OutRemainingTime, CountDownOutput, LatentInfo)
				);
		}
	}

	//输入执行为取消。
	if (CountDownInput == EWarriorCountDownActionInput::Cancel)
	{
		//取消LatentAction。
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

int32 UWarriorFunctionLibrary::RandomIndexByWeights(const TArray<int32>& Weights, int32 WeightTotal)
{
	if (Weights.IsEmpty()) return 0;

	//没有传入有效总权重时，自行计算。
	if (WeightTotal <= 0)
	{
		WeightTotal = 0;
		for (int32 i = 0; i < Weights.Num(); i++)
		{
			WeightTotal += Weights[i];
		}
	}

	//随机数在总权重值内。
	const int32 RandomInt = FMath::RandRange(1, WeightTotal);

	//确认随机数命中区段。
	int32 Right = 0;
	for (int32 i = 0; i < Weights.Num(); i++)
	{
		Right += Weights[i];

		if (RandomInt <= Right)
		{
			return i;
		}
	}

	return 0;
}

int32 UWarriorFunctionLibrary::RandomIndexByWeightsForThree(const int32 Weight1, const int32 Weight2, const int32 Weight3)
{
	const int32 WeightTotal = Weight1 + Weight2 + Weight3;
	const int32 RandomInt = FMath::RandRange(1, WeightTotal);

	//确认随机数命中区段。
	if (RandomInt <= Weight1) return 0;
	if (RandomInt <= Weight1 + Weight2) return 1;
	return 2;
}

float UWarriorFunctionLibrary::LerpLimitChangeMin(const float A, const float B, const float LimitChangeMin, const float Alpha)
{
	if (B == A) return B;
	
	float Change = Alpha * (B - A);

	//变化小于限制最小变化值
	if (LimitChangeMin > 0 && FMath::Abs(Change) < LimitChangeMin)
	{
		if ( Change > 0.f )
		{
			if (A + LimitChangeMin <= B)
			{
				Change = LimitChangeMin;
			}
			else
			{
				return B;
			}
		}
		else
		{
			if (A + LimitChangeMin >= B)
			{
				Change = -LimitChangeMin;
			}
			else
			{
				return B;
			}
		}
	}
	
	return A + Change;
}

#pragma endregion

#pragma region Gameplay

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	//在技能调用中按ESC结束游戏会导致空。
	//heck(InActor);
	if (!InActor) return nullptr;

	UWarriorAbilitySystemComponent* AbilitySystemComponent = Cast<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	return AbilitySystemComponent;
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if(ASC && !ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	if(ASC && ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	return ASC && ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
	EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if(IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	return CombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);
	
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		//自身队伍ID和目标队伍ID不相等。
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	
	return false;
}

float UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return  InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UWarriorFunctionLibrary::ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim,
	float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	//获取攻击者和受害者数据。
	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	//计算角度差。
	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	//计算叉乘。
	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

	//根据叉乘设定正负号。
	if (CrossResult.Z < 0.0f)
	{
		OutAngleDifference *= -1.f;
	}

	//确认方向
	if (OutAngleDifference >= -45.f)
	{
		if (OutAngleDifference <= 45.f)
			return FGameplayTag(WarriorGameplayTags::Shared_Status_HitReact_Front);
		if (OutAngleDifference <= 135.f)
			return FGameplayTag(WarriorGameplayTags::Shared_Status_HitReact_Right);
	}
	if (OutAngleDifference < -45.f)
	{
		if (OutAngleDifference >= -135.f)
			return FGameplayTag(WarriorGameplayTags::Shared_Status_HitReact_Left);
	}
		
	return FGameplayTag(WarriorGameplayTags::Shared_Status_HitReact_Back);
}

bool UWarriorFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	//const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"), DotResult, DotResult < -0.2f ? TEXT("Valid Block.") : TEXT("Invalid Block."));
	//Debug::Print(DebugString, DotResult < -0.2f ? FColor::Green : FColor::Red);
	
	//TODO：根据技能要求有不同的防御有效范围。
	return DotResult < -0.2f;
}

bool UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	FGameplayEffectSpecHandle& InSpecHandle)
{
	UWarriorAbilitySystemComponent* SourceASC = NativeGetWarriorASCFromActor(InInstigator);
	UWarriorAbilitySystemComponent* TargetASC = NativeGetWarriorASCFromActor(InTargetActor);
	if (!SourceASC || !TargetASC) return false;
	
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

UWarriorGameInstance* UWarriorFunctionLibrary::GetWarriorGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UWarriorGameInstance>();
		}
	}

	return nullptr;
}

void UWarriorFunctionLibrary::SetInputMode(const UObject* WorldContextObject, const EWarriorInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;

	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if (!PlayerController) return;

	const FInputModeGameOnly GameOnlyMode;
	const FInputModeUIOnly UIOnlyMode;
	
	switch (InInputMode)
	{
	case EWarriorInputMode::GameOnly:
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
		break;
	case EWarriorInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;
		break;
	}
}

void UWarriorFunctionLibrary::SaveCurrentGameDifficulty(EWarriorGameDifficulty InDifficultyToSave)
{
	USaveGame* SaveGameObj = UGameplayStatics::CreateSaveGameObject(UWarriorSaveGame::StaticClass());

	if (UWarriorSaveGame* WarriorSaveGameObj = Cast<UWarriorSaveGame>(SaveGameObj))
	{
		WarriorSaveGameObj->SavedGameDifficulty = InDifficultyToSave;
		const bool bWasSaved = UGameplayStatics::SaveGameToSlot(WarriorSaveGameObj, WarriorGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		//Debug::Print(bWasSaved ? TEXT(" Difficulty Saved") : TEXT("Difficulty NOT Saved"));
	}
}

bool UWarriorFunctionLibrary::TryLoadSavedGameDifficulty(EWarriorGameDifficulty& OutSavedDifficulty)
{
	if (UGameplayStatics::DoesSaveGameExist(WarriorGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObj = UGameplayStatics::LoadGameFromSlot(WarriorGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
		if (const UWarriorSaveGame* WarriorSaveGameObj = Cast<UWarriorSaveGame>(SaveGameObj))
		{
			OutSavedDifficulty = WarriorSaveGameObj->SavedGameDifficulty;

			//Debug::Print(TEXT("Loading Successful"), FColor::Green);

			return true;
		}
	}

	//存储默认值。
	OutSavedDifficulty = EWarriorGameDifficulty::Easy;
	SaveCurrentGameDifficulty(OutSavedDifficulty);
	return false;
}

FGameplayAbilitySpec UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(
	const TSubclassOf<UGameplayAbility>& GameplayAbility, const TWeakObjectPtr<UObject> SourceObject,
	const int32 ApplyLevel)
{
	FGameplayAbilitySpec AbilitySpec(GameplayAbility); //技能。
	AbilitySpec.SourceObject = SourceObject; //来源。比如施法者。
	AbilitySpec.Level = ApplyLevel; //技能等级。可用于在配置表查询不同等级对应的不同数值。

	return AbilitySpec;
}

FGameplayAbilitySpec UWarriorFunctionLibrary::NativeGetGameplayAbilitySpec(
	const TSubclassOf<UGameplayAbility>& GameplayAbility, const TWeakObjectPtr<UObject> SourceObject,
	const int32 ApplyLevel, const FGameplayTag InputTag)
{
	FGameplayAbilitySpec AbilitySpec = NativeGetGameplayAbilitySpec(GameplayAbility, SourceObject, ApplyLevel);
	AbilitySpec.DynamicAbilityTags.AddTag(InputTag);

	return AbilitySpec;
}

#pragma endregion
