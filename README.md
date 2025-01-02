# Blur_ARPG_Warrior
being developed

## 简介

### 开发内容

1.基于UE的Gameplay框架的玩家英雄和敌人。
2.基于EnhancedInput的操作系统。
3.Locomotion动画状态机。
4.基于GAS的战斗系统。
5.UI。
6.AI。
6.1.Detour Crowd Avoidance.人群绕行避让。
6.2.Behavior Tree.行为树。

敌人消融Material和Niagara特效。

### 项目类结构图
TODO

### Debug指引
' 按键（Enter左侧）打开综合控制台。小键盘数字键开关显示内容。
游玩隐藏鼠标时 Shift + F1 显示鼠标。

#### 自动避障
控制台输入 ai.crowd.DebugSelectActor 1 后选中敌人Actor查看自动避障Debug绘制。

#### GAS
控制台输入 showdebug abilitysystem 后，来打开Debug界面。使用 PageUp 和 PageDown 按钮来切换目标。

需要在DefaultGame.ini中添加配置。
[/Script/GameplayAbilities.AbilitySystemGlobals]
bUseDebugTargetFromHud = true

#### NavMesh
按 P 开关显示NavMesh生成范围。BT行为树的MoveTo功能需要NavMesh的支持。