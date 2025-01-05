// Blur Feng All Rights Reserved.


#include "Items/PickUps/WarriorPickUpBase.h"

#include "Components/SphereComponent.h"

// Sets default values
AWarriorPickUpBase::AWarriorPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>("PickUpCollisionSphere");
	SetRootComponent(PickUpCollisionSphere);
	PickUpCollisionSphere->InitSphereRadius(50.f);
	PickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpCollisionSphereBeginOverlap);
}

void AWarriorPickUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
}
