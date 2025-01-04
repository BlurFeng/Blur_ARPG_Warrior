// Blur Feng All Rights Reserved.


#include "Components/UI/EnemyUIComponent.h"

#include "Widgets/WarriorWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UWarriorWidgetBase* InWidgetToRegister)
{
	if (EnemyDrawnWidgets.Contains(InWidgetToRegister)) return;
	
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty()) return;

	for (UWarriorWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (!DrawnWidget) continue;

		DrawnWidget->RemoveFromParent();
	}
}
