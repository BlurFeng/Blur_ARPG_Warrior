# Blur_ARPG_Warrior
being developed

## 简介


### 开发内容

#### 游戏Gameplay框架
基于UE的Gameplay框架的玩家英雄和敌人。
基于DataAssets的初始化配置。

#### 移动功能和动画状态机
Locomotion动画状态机。

#### GAS技能系统
基于GAS的技能系统和角色状态管理。
MotionWarping用于技能中的位移变化。
GameplayTag用于游戏流程管理和通讯。

#### UI界面
UMG。
英雄状态。
敌人状态。

#### 英雄
##### 输入系统
基于EnhancedInput的操作系统。
##### 装备功能
武器的配备和卸下。不同武器赋予角色不同的技能。
##### 目标锁定
锁定敌人，并在不同敌人间切换。
##### 攻击连击
轻攻击。
重攻击。
##### 防御
防御。
完美防御和防御反击。
##### 翻滚
翻滚技能。

#### 敌人
近战敌人，远程敌人。Boss。
##### AI
Behavior Tree.行为树。
Detour Crowd Avoidance.人群绕行避让。
EQS环境查询系统。
##### 攻击技能
近战攻击和远程攻击。随机选择攻击方式。
##### 召唤技能
召唤敌人角色。

#### 材质和特效
受击效果。
敌人消融Material和Niagara特效。用于进场和退场动画。


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

#### EQS
使用 EQSTestingPawn 并配置使用的 EQ。可以在场景中可视化数据。