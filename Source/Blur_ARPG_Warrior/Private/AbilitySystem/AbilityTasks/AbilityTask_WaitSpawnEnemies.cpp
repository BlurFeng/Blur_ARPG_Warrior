// Blur Feng All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/AssetManager.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility,
                                                                               FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn,
                                                                               const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	//Super::Activate();

	//激活时绑定委托。
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	//销毁时移除委托。
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	Delegate.Remove(DelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	//收到GameplayEvent。

	//确认配置了有效的生成敌人类型。
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		//异步加载。在OnEnemyClassLoaded回调中处理之后的逻辑。
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded));
	}
	else
	{
		//确认可以进行技能任务广播。确认技能任然在激活状态。
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	//确认参数是否有效。
	if (!LoadedClass || !World)
	{
		//确认可以进行技能任务广播。确认技能任然在激活状态。
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}

		EndTask();
		return;
	}

	TArray<AWarriorEnemyCharacter*> SpawnedEnemies;
	const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

	//生成参数。
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{
		//Notes：K2_ 前缀的含义是蓝图可调用方法。

		//获取一个随机的可到达位置。我们通过UNavigationSystemV1类来随机获取一个真正有效的位置。
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);

		//向上移动一段距离，防止生成的敌人卡在地板上。
		RandomLocation += FVector(0.f, 0.f, 100.f);
		
		//生成敌人。
		if (AWarriorEnemyCharacter* SpawnEnemy = World->SpawnActor<AWarriorEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParams))
		{
			SpawnedEnemies.Add(SpawnEnemy);
		}
	}

	//广播生成结束委托。
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}
	}
	
	EndTask();
}
