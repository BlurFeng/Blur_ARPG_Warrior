// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"

class UEnemyUIComponent;
class UHeroUIComponent;
class UPawnUIComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};

//人物UI接口，用于人物和UI之间进行通信。
class BLUR_ARPG_WARRIOR_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//获取PawnUI组件。
	virtual UPawnUIComponent* GetPawnUIComponent() const = 0;

	//此接口不是必须实现的，由继承的类决定是否实现内容。
	//获取英雄UI组件
	virtual UHeroUIComponent* GetHeroUIComponent() const;

	//获取敌人UI组件
	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
};
