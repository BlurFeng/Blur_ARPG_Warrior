// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetTaskMemory
{
	//自身Pawn。
	TWeakObjectPtr<APawn> OwningPawn;

	//目标Actor。
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const { return OwningPawn.IsValid() && TargetActor.IsValid(); }

	void Reset() { OwningPawn.Reset(); TargetActor.Reset(); }
};

//旋转面向目标Task。
UCLASS()
class BLUR_ARPG_WARRIOR_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_RotateToFaceTarget();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const;

	//角度精度。
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float AnglePrecision;

	//旋转间隔速度。
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float RotationInterpSpeed;

	//面向的目标。
	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector InTargetToFaceKey;
};


