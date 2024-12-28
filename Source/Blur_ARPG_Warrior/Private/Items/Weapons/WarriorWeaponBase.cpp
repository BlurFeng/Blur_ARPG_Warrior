// Blur Feng All Rights Reserved.


#include "Items/Weapons/WarriorWeaponBase.h"

#include "Components/BoxComponent.h"

// Sets default values
AWarriorWeaponBase::AWarriorWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//创建武器Mesh
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	//创建武器碰撞盒
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	//默认设置为无碰撞，仅在需要进行伤害检测时开启
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
