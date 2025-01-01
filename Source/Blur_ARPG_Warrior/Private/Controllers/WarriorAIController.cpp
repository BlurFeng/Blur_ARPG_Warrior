// Blur Feng All Rights Reserved.


#include "Controllers/WarriorAIController.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "WarriorDebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"

//Notes：通过构造函数重载成员变量类型
//通过这种方式。我们允许重载父类的一些成员变量类型为其子类。
//而在蓝图中，我们可以直接通过Details面板中操作更改成员变量的类型。

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	//创建AI感知配置。
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; //检测敌人
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; //检测队友
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false; //检测中立
	AISenseConfig_Sight->SightRadius = 5000.f; //视野半径
	AISenseConfig_Sight->LoseSightRadius = 0.f; //丢失视野半径
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; //周边视觉角度

	//创建AI感知组件
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight); //使用配置
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass()); //设置感知类型
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	//设置队伍ID
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());

	//确认是不同队伍的目标
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile; //敌对
	}

	//相同队伍
	return ETeamAttitude::Friendly; //队友
}

void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		//开关人群避让状态
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		//根据配置设置人群避障算法质量。
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange); //设置碰撞查询范围
	}
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		//Debug::Print(Actor->GetActorNameOrLabel() + TEXT(" was sensed"), FColor::Green);
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			//设置目标到自身黑板的 TargetActor
			BlackboardComponent->SetValueAsObject("TargetActor", Actor);
		}
	}	
}
