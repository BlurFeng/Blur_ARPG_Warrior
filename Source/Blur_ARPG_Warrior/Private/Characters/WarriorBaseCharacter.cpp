// Blur Feng All Rights Reserved.


#include "Characters/WarriorBaseCharacter.h"

// Sets default values
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//是否接受贴花。关闭。因为会对地面使用一些贴花投射。
	GetMesh()->bReceivesDecals = false;
}