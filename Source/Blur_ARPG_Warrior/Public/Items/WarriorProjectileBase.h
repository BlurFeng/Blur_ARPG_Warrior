// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "WarriorProjectileBase.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UNiagaraComponent;
struct FGameplayEventData;

//炮弹造成伤害方式。
UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	//当击中时。
	OnHit,
	//当开始穿过时。
	OnBeginOverlap,
};

UCLASS()
class BLUR_ARPG_WARRIOR_API AWarriorProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWarriorProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//炮弹碰撞器。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	//炮弹Niagara特效。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	//炮弹移动组件。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	//炮弹伤害策略，击中还是穿过。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy;

	//Notes：ExposeOnSpawn 暴露数据在生成时。
	//使用 ExposeOnSpawn 可以让某个成员参数在蓝图中Spawn生成时，直接显示在Spawn节点中供传入。
	
	//炮弹伤害效果配置数据。
	UPROPERTY(BlueprintReadOnly, category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

	//当炮弹撞击时。
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//当炮弹开始穿过时。
	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//生成炮弹击中特效事件。由蓝图实现。
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

private:
	
	//应用炸弹伤害GE到目标Pawn。
	void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);
};
