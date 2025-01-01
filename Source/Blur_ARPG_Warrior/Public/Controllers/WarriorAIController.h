// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WarriorAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

//AI组件基类。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWarriorAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface
protected:

	//AI感知组件。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	//AI感知配置。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	//敌人感知更新时回调。
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};