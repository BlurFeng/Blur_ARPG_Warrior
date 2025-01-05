// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorPickUpBase.generated.h"

class USphereComponent;

//可拾取道具基类。
UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWarriorPickUpBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick Up Interaction")
	USphereComponent* PickUpCollisionSphere;

	/// 有其他Actor和拾取道具碰撞和重叠时。
	/// @param OverlappedComponent 
	/// @param OtherActor 
	/// @param OtherComp 
	/// @param OtherBodyIndex 
	/// @param bFromSweep 
	/// @param SweepResult 
	UFUNCTION()
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                 UPrimitiveComponent* OtherComp, int OtherBodyIndex,
	                                                 bool bFromSweep,
	                                                 const FHitResult& SweepResult);
};
