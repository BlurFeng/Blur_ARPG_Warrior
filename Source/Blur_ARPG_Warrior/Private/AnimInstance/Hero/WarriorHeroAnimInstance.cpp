// Blur Feng All Rights Reserved.


#include "AnimInstance/Hero/WarriorHeroAnimInstance.h"

#include "Characters/WarriorHeroCharacter.h"

void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if(OwningCharacter)
	{
		OwningHeroCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
	}
}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if(bShouldEnterRelaxState && bHasAcceleration)
	{
		IdleElapsedTimer = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTimer += DeltaSeconds;

		bShouldEnterRelaxState = IdleElapsedTimer >= EnterRelaxStateThreshold;
	}
}
