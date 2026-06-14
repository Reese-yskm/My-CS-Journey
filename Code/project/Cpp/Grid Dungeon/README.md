# Grid Dungeon Lab

一个用于训练状态设计、回合流程和多对象交互的 C++ 终端回合制小游戏。

## 1. 项目简介

`Grid Dungeon` 是一个小型终端回合制地图游戏。

玩家被困在一个地下迷宫中，需要拿到钥匙、打开出口并逃离。迷宫中有墙壁、陷阱、守卫和水晶。每输入一个命令，游戏推进一个回合。

这个项目不是为了做一个大型游戏，而是为了训练以下能力：

```text
二维地图表示
对象状态设计
回合制流程拆分
玩家动作、环境更新、敌人行动、胜负判定
多对象扫描顺序
延迟生效机制
冷却机制
状态保留与状态消耗
边界情况测试
```

本项目建议使用 **C++** 实现，但不使用复杂框架，不使用图形界面，只使用终端输入输出。

------

## 2. 推荐技术范围

建议使用：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
```

可以使用：

```cpp
std::vector
std::string
enum class
struct
class
```

暂时不建议使用：

```text
继承
多态
模板
智能指针
图形库
第三方库
```

这个项目重点不是炫技，而是把状态和流程写清楚。

------

## 3. 地图大小

推荐固定地图大小：

```cpp
const int ROWS = 10;
const int COLS = 10;
```

地图使用二维数组或二维 vector：

```cpp
std::vector<std::vector<Tile>> map;
```

推荐使用：

```cpp
using Grid = std::vector<std::vector<Tile>>;
```

------

## 4. 地图符号建议

你可以自行调整显示符号，但建议先使用：

```text
空地 EMPTY             "   "
墙 WALL               "###"
玩家 PLAYER           " P "
钥匙 KEY              " K "
锁着的出口 EXIT_LOCKED "[X]"
打开的出口 EXIT_OPEN   "[O]"
隐藏陷阱 HIDDEN_TRAP   " . "
激活陷阱 ARMED_TRAP    " ! "
火焰陷阱 FIRE_TRAP     "***"
沉睡守卫 SLEEPING      "zzz"
警戒守卫 ALERT         " G "
疲惫守卫 TIRED         " g "
水晶 CRYSTAL           " C "
```

如果玩家站在特殊地格上，可以特殊显示：

```text
玩家站在陷阱火焰上："<P>"
玩家站在出口上：     "[P]"
玩家站在激活陷阱上："<P>"
```

------

## 5. 核心数据结构建议

### 5.1 地图类型

```cpp
enum class TileType {
    Empty,
    Wall,
    Key,
    ExitLocked,
    ExitOpen,
    Trap,
    Guard,
    Crystal
};
```

------

### 5.2 陷阱状态

```cpp
enum class TrapState {
    None,
    Hidden,
    Armed,
    Fire
};
```

含义：

```text
None   ：没有陷阱
Hidden ：隐藏陷阱，玩家踩到后变成 Armed
Armed  ：已激活，下一阶段会变成 Fire
Fire   ：火焰状态，会杀死玩家，之后消失
```

这个机制用来训练“延迟生效”。

------

### 5.3 守卫状态

```cpp
enum class GuardState {
    None,
    Sleeping,
    Alert,
    Tired
};
```

含义：

```text
None     ：没有守卫
Sleeping ：沉睡，不行动
Alert    ：警戒，会攻击或巡逻
Tired    ：疲惫，满足条件时跳过一次行动后恢复 Alert
```

这个机制用来训练“状态机”和“冷却”。

------

### 5.4 Tile 结构体

```cpp
struct Trap {
    TrapState state = TrapState::None;
    bool newlyArmed = false;
};

struct Guard {
    GuardState state = GuardState::None;
    bool skipNextAction = false;
    bool actedThisTurn = false;
};

struct Tile {
    TileType type = TileType::Empty;
    Trap trap;
    Guard guard;
};
```

说明：

```text
TileType 决定这个格子的主要类型。
Trap 保存陷阱状态。
Guard 保存守卫状态。
```

虽然一个格子通常不会同时有陷阱和守卫，但保留结构体字段可以让代码更清楚。

------

### 5.5 Player 结构体

```cpp
struct Player {
    int row = 0;
    int col = 0;
    bool hasKey = false;
    bool alive = true;
    bool won = false;
};
```

玩家不要直接存成地图上的一种 TileType。
推荐单独保存玩家坐标。

原因：

```text
玩家可能站在钥匙、出口、陷阱等特殊格子上。
如果把玩家直接写进地图，会覆盖原来的地格信息。
```

------

## 6. 游戏整体流程

每一回合分为四个步骤：

```text
1. Player Step
   读取玩家命令，执行移动或交互。

2. Upkeep Step
   更新陷阱、火焰、延迟效果等环境状态。

3. Guard Step
   所有守卫按照从左上到右下的顺序行动。

4. End Step
   判断玩家是否失败或胜利。
```

主循环大致是：

```text
Game Started!
Enter command:
读取命令
执行 Player Step
执行 Upkeep Step
执行 Guard Step
打印地图
执行 End Step
如果游戏结束，输出结果
否则继续读取下一条命令
```

注意：**胜负判定统一放在 End Step。**

------

# Stage 1：地图初始化与玩家移动

## Stage 1.1 - 初始化地图与玩家

### 目标

创建固定大小地图，初始化为空地，并生成玩家。

### 输入格式

```text
Player starting position: [row] [col]
```

### 行为说明

读取玩家位置，若位置合法，则打印地图。

### 边界情况

玩家位置必须在地图内。

非法位置可以直接输出：

```text
Invalid player position!
```

### 建议函数

```cpp
Grid createMap();
bool inMap(int row, int col);
void printMap(const Grid& map, const Player& player);
```

### 自测样例

```text
输入:
0 0

预期:
玩家显示在左上角。
```

### 思考问题

```text
为什么 Player 不应该直接作为 TileType 存进地图？
如果玩家站在 Key 上，地图格子应该是什么？
```

------

## Stage 1.2 - 添加墙壁

### 目标

实现墙壁生成。

### 输入格式

```text
How many walls: [count]
[row] [col]
[row] [col]
...
```

### 新增规则

墙壁不可通行。

墙不能放在：

```text
地图外
玩家当前位置
已经被占用的位置
```

### 建议函数

```cpp
bool canPlaceWall(const Grid& map, const Player& player, int row, int col);
void placeWalls(Grid& map, const Player& player);
```

### 思考问题

```text
canPlaceWall 是否应该修改地图？
为什么判断函数和执行函数分开更清楚？
```

------

## Stage 1.3 - 玩家移动命令循环

### 目标

实现玩家移动。

### 命令

```text
w：上
a：左
s：下
d：右
i：原地等待
```

### 行为说明

玩家可以移动到 `Empty`。

玩家不能移动到：

```text
地图外
Wall
```

每次命令后打印地图。

### 建议函数

```cpp
bool playerCanMoveTo(const Grid& map, int row, int col);
void movePlayer(Grid& map, Player& player, char command);
void commandLoop(Grid& map, Player& player);
```

### 自测样例

```text
玩家初始在 (0, 0)

输入:
d
s
a

预期:
玩家依次移动到 (0,1), (1,1), (1,0)。
```

### 思考问题

```text
i 命令是否算一个完整回合？
即使玩家不移动，后续 Upkeep 和 Guard Step 要不要执行？
```

建议答案：算。等待也是一个完整回合。

------

# Stage 2：钥匙、出口与基础胜负

## Stage 2.1 - 添加钥匙与出口

### 目标

地图中加入钥匙和出口。

### 输入格式

```text
Key position: [row] [col]
Exit position: [row] [col]
```

### 新增规则

玩家走到钥匙上：

```text
player.hasKey = true
Key -> Empty
```

玩家拿到钥匙后，出口从：

```text
ExitLocked -> ExitOpen
```

### 建议函数

```cpp
bool canPlaceItem(const Grid& map, const Player& player, int row, int col);
void placeKeyAndExit(Grid& map, const Player& player);
void updateExit(Grid& map, const Player& player);
void handlePlayerTile(Grid& map, Player& player);
```

### 行为说明

玩家移动后，需要检查当前格子：

```text
如果是 Key，拾取钥匙。
如果有钥匙，打开出口。
```

### 思考问题

```text
玩家拿到钥匙后，出口应该立刻打开吗？
打开出口的逻辑应该写在 movePlayer 里，还是单独写 updateExit？
```

建议：单独写 `updateExit`，让移动函数只负责移动。

------

## Stage 2.2 - 胜利判定

### 目标

玩家站在打开的出口上时获胜。

### 胜利输出

```text
You escaped the dungeon!
```

### 规则

如果玩家位于 `ExitOpen` 上，则在 End Step 判定胜利。

### 建议函数

```cpp
bool playerWon(const Grid& map, const Player& player);
bool endStep(const Grid& map, Player& player);
```

### 思考问题

```text
为什么胜利判定不直接写在 movePlayer 里？
如果以后同一回合玩家既站在出口上又死亡，应该谁优先？
```

建议规则：失败优先。

------

# Stage 3：陷阱与延迟生效

这一阶段开始训练状态机。

------

## Stage 3.1 - 添加隐藏陷阱

### 目标

地图中加入隐藏陷阱。

### 输入格式

```text
How many traps: [count]
[row] [col]
[row] [col]
...
```

### 新增规则

陷阱初始状态是：

```cpp
TrapState::Hidden
```

玩家可以走到隐藏陷阱上。

玩家踩到隐藏陷阱后：

```text
Hidden -> Armed
```

玩家不会立刻死亡。

### 建议函数

```cpp
bool canPlaceTrap(const Grid& map, const Player& player, int row, int col);
void placeTraps(Grid& map, const Player& player);
void triggerTrap(Grid& map, const Player& player);
```

### 思考问题

```text
玩家踩到陷阱为什么不立刻死亡？
Hidden -> Armed -> Fire 这种延迟状态有什么好处？
```

------

## Stage 3.2 - Upkeep Step：陷阱爆发

### 目标

实现陷阱随回合更新。

### 新增规则

Upkeep Step 中陷阱状态更新：

```text
Fire -> Empty
Armed -> Fire
```

如果玩家站在 `Fire` 上，则玩家死亡。

### 推荐更新顺序

为了避免同一回合连续变化，推荐两阶段更新：

```text
1. 如果玩家当前站在 Fire 上，玩家死亡。
2. 所有 Fire 变回 Empty。
3. 所有 Armed 变成 Fire。
```

这样表示：

```text
玩家踩到 Hidden 后变 Armed。
下一次 Upkeep，Armed 变 Fire。
Fire 存在一整回合。
如果玩家不离开，下一次 Upkeep 死亡。
```

### 建议函数

```cpp
void updateTrapStates(Grid& map);
bool playerOnFire(const Grid& map, const Player& player);
void upkeepStep(Grid& map, Player& player);
```

### 边界情况

```text
玩家踩到 Hidden 的同一回合不能死亡。
玩家如果及时离开 Fire，不会死亡。
Fire 不能在同一次 Upkeep 中立刻消失又重新生成。
```

### 思考问题

```text
为什么不能一边扫描地图一边让 Hidden -> Armed -> Fire 连续变化？
为什么 Upkeep Step 要独立出来？
```

------

## Stage 3.3 - 失败判定

### 目标

玩家死亡时游戏结束。

### 失败输出

```text
You were defeated!
```

### 回合顺序

现在主循环应为：

```text
Player Step
Upkeep Step
Print Map
End Step
```

如果玩家死于 Upkeep，仍然先打印地图，再输出失败信息。

### 建议函数

```cpp
bool playerAlive(const Player& player);
```

### 思考问题

```text
为什么死亡判定放在 End Step，而不是在 Upkeep 中直接 return？
以后加入 Guard Step 后，玩家死亡后守卫是否还要行动？
```

------

# Stage 4：守卫状态机与多对象行动

这一阶段是核心训练阶段。

------

## Stage 4.1 - 生成沉睡守卫

### 目标

添加多个守卫，初始状态为 `Sleeping`。

### 输入格式

```text
How many guards: [count]
[row] [col]
[row] [col]
...
```

### 新增规则

守卫不能放在：

```text
地图外
玩家位置
墙
钥匙
出口
陷阱
其他守卫
```

守卫初始状态：

```cpp
GuardState::Sleeping
```

### 建议函数

```cpp
bool canPlaceGuard(const Grid& map, const Player& player, int row, int col);
void placeGuards(Grid& map, const Player& player);
```

### 思考问题

```text
守卫应该存在 map[row][col].guard 里，还是单独用 vector<GuardEntity> 存？
二维地图扫描有什么简单之处？
如果守卫会移动，二维地图扫描会带来什么问题？
```

------

## Stage 4.2 - 惊醒守卫

### 目标

实现范围警报。

### 新增规则

当玩家触发陷阱、拿到钥匙或剪切水晶时，以事件发生位置为中心，5x5 范围内的守卫被惊醒：

```text
Sleeping -> Alert
```

刚被惊醒的守卫本回合不行动。

因此需要设置：

```cpp
guard.skipNextAction = true;
```

### 建议函数

```cpp
void alertGuards(Grid& map, int centerRow, int centerCol);
```

### 边界情况

```text
警报范围可能超出地图边界。
多个守卫可能同时被惊醒。
已经 Alert 的守卫保持 Alert。
Tired 的守卫保持 Tired。
```

### 思考问题

```text
为什么刚被惊醒的守卫不应该立刻行动？
skipNextAction 应该什么时候清掉？
是轮到守卫就清掉，还是有行动机会时才清掉？
```

------

## Stage 4.3 - 守卫攻击

### 目标

警戒守卫可以攻击附近玩家。

### 新增规则

Guard Step 中，从左上到右下扫描所有守卫。

对于每个 `Alert` 守卫：

```text
如果玩家在它的 8 邻域内，守卫攻击玩家。
玩家不会立刻终止流程，而是在 End Step 判负。
攻击后守卫变为 Tired。
```

对于 `Tired` 守卫：

```text
暂时先简单处理：
跳过本回合，然后变回 Alert。
```

后面 Stage 4.5 再加入更精细规则。

### 建议函数

```cpp
bool playerNearGuard(int guardRow, int guardCol, const Player& player);
bool guardAttack(Grid& map, Player& player, int guardRow, int guardCol);
void guardStep(Grid& map, Player& player);
```

### 扫描顺序

必须按照：

```text
row 从 0 到 ROWS - 1
col 从 0 到 COLS - 1
```

### 思考问题

```text
为什么多个守卫行动时要规定扫描顺序？
如果第一个守卫已经攻击玩家，后面的守卫还要不要行动？
为什么死亡判定最好留到 End Step？
```

建议：后面的守卫继续行动，End Step 再统一判负。

------

## Stage 4.4 - 守卫巡逻

### 目标

如果守卫没有攻击玩家，就尝试移动。

### 新增规则

`Alert` 守卫如果不能攻击玩家，则尝试移动。

移动优先级：

```text
UP -> RIGHT -> DOWN -> LEFT
```

守卫只能移动到 `Empty`。

不能移动到：

```text
地图外
Wall
Key
Exit
Trap
Crystal
Guard
玩家当前位置
```

移动后守卫进入：

```text
Tired
```

### 建议函数

```cpp
bool canGuardMoveTo(const Grid& map, const Player& player, int row, int col);
bool moveGuardOnce(Grid& map, const Player& player, int guardRow, int guardCol);
```

### 关键难点

如果你边扫描边移动守卫，可能出现：

```text
守卫从后面移动到前面，被同一轮扫描再次扫到。
守卫一回合行动多次。
```

为了解决这个问题，使用：

```cpp
guard.actedThisTurn
```

Guard Step 开始时：

```text
清空所有守卫的 actedThisTurn。
```

守卫行动后：

```text
actedThisTurn = true;
```

扫描到已经行动过的守卫时跳过。

### 建议函数

```cpp
void resetGuardActions(Grid& map);
```

### 思考问题

```text
为什么守卫移动后可能被重复扫描？
如果守卫只会向下移动，会不会出现同样问题？
actedThisTurn 解决了什么？
```

------

## Stage 4.5 - 精细冷却：有机会才消耗 skip

### 目标

实现更严谨的守卫冷却规则。

### 新增规则

守卫进入 `Tired` 后，并不是轮到它就一定恢复。

规则改为：

```text
如果 Tired 守卫当前有行动机会：
    跳过这次行动
    Tired -> Alert
否则：
    保持 Tired
```

“有行动机会”指：

```text
玩家在攻击范围内
或者守卫至少有一个方向可以移动
```

### 建议函数

```cpp
bool guardHasAction(const Grid& map, const Player& player, int row, int col);
void handleTiredGuard(Grid& map, const Player& player, int row, int col);
```

### 类比

这个阶段专门对应你在 CS Flowers 中遇到的：

```text
skip_next_defense 不能轮到就清掉。
必须有防御机会时，跳过才算真正消耗 skip。
```

### 思考问题

```text
什么叫“有行动机会”？
如果没有行动机会，为什么不能恢复 Alert？
这个规则如何避免状态被过早消耗？
```

------

# Stage 5：水晶与连锁反应

这一阶段是提高部分，可以最后做。

------

## Stage 5.1 - 添加水晶和剪切命令

### 目标

加入水晶 `Crystal`，并实现剪切命令。

### 新增命令

```text
c [direction]
```

方向：

```text
w a s d
```

玩家可以剪切相邻格子的水晶。

### 新增规则

剪切水晶后：

```text
Crystal -> Empty
以水晶为中心 alertGuards
附近 Hidden 陷阱变 Armed
```

### 建议函数

```cpp
void cutTile(Grid& map, Player& player, char direction);
void crystalBurst(Grid& map, int row, int col);
```

### 思考问题

```text
剪切命令属于 Player Step 吗？
crystalBurst 应该写在 cutTile 里，还是单独封装？
为什么复杂事件最好单独写函数？
```

------

## Stage 5.2 - 连锁陷阱的延迟生效

### 目标

处理水晶激活陷阱后的延迟问题。

### 新增规则

水晶爆发会让 5x5 范围内：

```text
Hidden -> Armed
```

但是本回合刚变成 `Armed` 的陷阱，不能在同一个 Upkeep Step 中立刻变成 `Fire`。

因此使用：

```cpp
trap.newlyArmed = true;
```

Upkeep 处理时：

```text
如果 Armed 且 newlyArmed == true：
    newlyArmed = false
    本回合不变 Fire

如果 Armed 且 newlyArmed == false：
    Armed -> Fire
```

### 思考问题

```text
为什么需要 newlyArmed？
如果没有 newlyArmed，什么情况下陷阱会同一回合连续变化？
这和“刚被惊醒的守卫本回合不行动”有什么相似之处？
```

------

## Stage 5.3 - 最终胜负优先级

### 目标

统一所有胜负冲突。

### 规则

如果同一回合同时发生：

```text
玩家站在 ExitOpen 上
玩家被陷阱或守卫杀死
```

则失败优先。

End Step 顺序：

```text
如果 player.alive == false：
    输出 You were defeated!
否则如果 playerWon(map, player)：
    输出 You escaped the dungeon!
否则继续游戏
```

### 思考问题

```text
为什么必须明确胜负优先级？
如果失败和胜利判断分散在不同函数中，会发生什么问题？
为什么 End Step 应该集中处理胜负？
```

------

# 推荐函数组织

## 地图相关

```cpp
Grid createMap();
bool inMap(int row, int col);
void printMap(const Grid& map, const Player& player);
std::string tileToString(const Tile& tile);
```

## 玩家相关

```cpp
bool playerCanMoveTo(const Grid& map, int row, int col);
void movePlayer(Grid& map, Player& player, char command);
void handlePlayerTile(Grid& map, Player& player);
bool playerWon(const Grid& map, const Player& player);
```

## 设置阶段

```cpp
void placeWalls(Grid& map, const Player& player);
void placeKeyAndExit(Grid& map, const Player& player);
void placeTraps(Grid& map, const Player& player);
void placeGuards(Grid& map, const Player& player);
void placeCrystals(Grid& map, const Player& player);
```

## 陷阱相关

```cpp
void triggerTrap(Grid& map, const Player& player);
bool playerOnFire(const Grid& map, const Player& player);
void updateTrapStates(Grid& map);
void upkeepStep(Grid& map, Player& player);
```

## 守卫相关

```cpp
void alertGuards(Grid& map, int centerRow, int centerCol);
bool playerNearGuard(int guardRow, int guardCol, const Player& player);
bool guardAttack(Grid& map, Player& player, int row, int col);
bool canGuardMoveTo(const Grid& map, const Player& player, int row, int col);
bool moveGuardOnce(Grid& map, const Player& player, int row, int col);
bool guardHasAction(const Grid& map, const Player& player, int row, int col);
void resetGuardActions(Grid& map);
void guardStep(Grid& map, Player& player);
```

## 回合流程

```cpp
void playerStep(Grid& map, Player& player, char command);
void upkeepStep(Grid& map, Player& player);
void guardStep(Grid& map, Player& player);
bool endStep(const Grid& map, Player& player);
void commandLoop(Grid& map, Player& player);
```

------

# 推荐主流程

程序整体结构建议：

```text
1. 创建地图
2. 读取玩家位置
3. 放置墙
4. 放置钥匙和出口
5. 放置陷阱
6. 放置守卫
7. 放置水晶
8. 打印初始地图
9. 进入 commandLoop
```

命令循环：

```text
Game Started!
Enter command:

while 成功读取命令:
    Player Step
    Upkeep Step
    Guard Step
    Print Map
    End Step
    如果游戏结束，退出
    否则继续提示 Enter command:
```

------

# 每个阶段开始前的固定思考模板

每次写新阶段前，先在笔记里回答：

```text
1. 这一阶段新增了哪些状态？
2. 状态属于 Player、Tile、Trap 还是 Guard？
3. 状态在哪个 Step 改变？
4. 本回合改变的状态，会不会影响本回合后续 Step？
5. 多个对象行动时扫描顺序是什么？
6. 是否存在“跳过一次行动”的机制？
7. 这个跳过状态是轮到就消耗，还是有机会才消耗？
8. 胜利和失败如果同时发生，谁优先？
```

------

# 自测建议

你可以自己建一个目录：

```text
tests/
  stage1_move.in
  stage2_key_exit.in
  stage3_trap_delay.in
  stage4_guard_alert.in
  stage4_guard_move.in
  stage4_tired_skip.in
  stage5_crystal_chain.in
```

每个测试先写自然语言预期，不急着写完整 `.ans`。

例如：

```text
测试名：stage4_tired_skip

场景：
守卫处于 Tired。
四周都是墙，没有移动机会，玩家也不在附近。

预期：
守卫保持 Tired，不恢复 Alert。

然后玩家移开某个障碍，使守卫有移动机会。

预期：
守卫这一回合跳过行动，并从 Tired 恢复 Alert。
下一回合才真正行动。
```

------

# 实现建议

这个项目你不要追求一天写完。建议节奏是：

```text
一天只做一个 Stage 或半个 Stage。
每个 Stage 先写规则理解。
再写伪代码。
再写函数。
最后自己设计 2~3 个测试。
```

遇到 bug 时，先不要直接问“怎么改”，而是先问自己：

```text
当前回合执行到哪一步？
这个状态本该在哪一步改变？
我是不是提前消耗了某个 skip？
我是不是边扫描边修改导致对象行动多次？
我是不是把胜负判定写得太早？
```

------

# 项目真正训练目标

完成这个项目后，你应该能更熟练地处理：

```text
状态机
回合制系统
对象行动顺序
延迟生效
冷却与跳过
多对象互相影响
胜负优先级
```

这就是你在 CS Flowers Stage 3 里真正遇到的难点。

这个项目没有 ddl，没有分数压力。
目标不是快，而是你自己真的把每个状态和每个回合顺序想清楚。