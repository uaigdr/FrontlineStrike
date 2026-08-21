# FrontlineStrike

第三人称战术射击游戏 — 个人独立开发（UE 5.8 / C++）

## 玩法

- 第三人称射击：射线命中判定、粒子命中反馈、伤害结算（ApplyDamage / TakeDamage 管线）、敌人受击与死亡
- 敌人 AI：AIController + 行为树（BehaviorTree）+ 黑板（Blackboard），视线检测、目标聚焦与追击
- 动画表现：动画蓝图状态机、移动混合空间（BlendSpace）、瞄准偏移（AimOffset）
- 输入：Enhanced Input 动作映射

## 技术栈

- Unreal Engine 5.8（C++ / 蓝图混合开发）
- 行为树 / 黑板 / NavMesh
- Enhanced Input / Niagara / UMG

## 项目结构

```
Source/FrontlineStrike/        C++ 源码（角色、武器、AI 控制器、GameMode）
Content/MyStuff/               自定义内容（角色、AI 控制器、行为树、动画蓝图、关卡）
Config/                        项目配置
FrontlineStrike.uproject       项目文件
```

## 运行环境

- Unreal Engine 5.8
- 双击 `FrontlineStrike.uproject` 打开（首次打开会自动编译 C++）

## 资产说明

- 角色模型与动画使用 Epic 免费资产 **Paragon: Wraith**
- 射击示例资产来自 UE 第三人称模板 ShooterGame 示例
