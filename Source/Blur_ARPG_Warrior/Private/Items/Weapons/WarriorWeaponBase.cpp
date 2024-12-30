// Blur Feng All Rights Reserved.


#include "Items/Weapons/WarriorWeaponBase.h"

#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"

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

	//绑定武器碰撞盒回调方法
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void AWarriorWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Notes：GetInstigator
	//在 Unreal Engine 中，GetInstigator() 是一个用于获取 Instigator（发起者） 的方法，
	//通常定义在 AActor 类中。它的作用是返回与该 Actor 相关联的一个 Pawn 或 Controller，作为它的 "发起者"（Instigator）。
	
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if(WeaponOwningPawn != HitPawn)
		{
			//Debug::Print(GetName() + TEXT("begin overlap with ") + HitPawn->GetName(), FColor::Green);

			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}

		//TODO: Inplement hit check for enemy characters
	}
}

void AWarriorWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if(WeaponOwningPawn != HitPawn)
		{
			//Debug::Print(GetName() + TEXT("end overlap with ") + HitPawn->GetName(), FColor::Red);

			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}

		//TODO: Inplement hit check for enemy characters
	}
}
