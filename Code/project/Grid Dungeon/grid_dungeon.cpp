#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ================================
// Constants
// ================================

const int ROWS = 11;
const int COLS = 11;

// ================================
// Enums
// ================================

// 地图格子的主要类型。
// 注意：Player 不作为 TileType，因为玩家可能站在 Key、Trap、Exit 等格子上。
// 所以玩家位置单独存在 Player 结构体里。
enum class TileType
{
    Empty,
    Wall,
    Key,
    ExitLocked,
    ExitOpen,
    Trap,
    Guard,
    Crystal
};

// 陷阱状态。
// Stage 3 以后会用到，目前先保留结构。
enum class TrapState
{
    None,
    Hidden,
    Armed,
    Fire
};

// 守卫状态。
// Stage 4 以后会用到，目前先保留结构。
enum class GuardState
{
    None,
    Sleeping,
    Alert,
    Tired
};

// ================================
// Structs
// ================================

struct Trap
{
    TrapState state = TrapState::None;

    // Stage 5 可能会用到：
    // 用来区分“本回合刚被激活的陷阱”，避免同一回合连续变化。
    bool newlyArmed = false;
};

struct Guard
{
    GuardState state = GuardState::None;

    // 用于“刚被惊醒的守卫跳过一次行动”
    bool skipNextAction = false;

    // 用于防止守卫移动后在同一轮被重复扫描
    bool actedThisTurn = false;
};

struct Tile
{
    TileType type = TileType::Empty;
    Trap trap;
    Guard guard;
};

struct Player
{
    int row = 0;
    int col = 0;
    bool hasKey = false;
    bool alive = true;
    bool won = false;
};

// 为了让类型更清楚，给二维地图起一个别名。
using Grid = vector<vector<Tile>>;

// ================================
// Function Prototypes
// ================================

Grid createMap();
bool inMap(int row, int col);
void initialisePlayer(Player &player);
void printMap(const Grid &map, const Player &player);
string tileToString(const Tile &tile);
bool playerAt(const Player &player, int row, int col);

// ================================
// Main
// ================================

int main()
{
    cout << "Welcome to Grid Dungeon!" << endl;

    Grid map = createMap();

    Player player;
    initialisePlayer(player);

    printMap(map, player);

    return 0;
}

// ================================
// Map Initialisation
// ================================

// 创建 ROWS x COLS 的地图。
// 每一个 Tile 默认都是 Empty，因为 Tile 结构体里已经给 type 设置了默认值。
Grid createMap()
{
    Grid map(ROWS, vector<Tile>(COLS));
    return map;
}

// 判断一个坐标是否在地图范围内。
// 后面所有需要访问 map[row][col] 的地方，都应该先用它检查。
bool inMap(int row, int col)
{
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

// ================================
// Player Initialisation
// ================================

// 读取玩家初始位置。
// 目前只做一次读取，如果非法就直接提示。
// 后面你可以改成 while 循环，要求重新输入。
void initialisePlayer(Player &player)
{
    cout << "Player starting position: ";

    cin >> player.row >> player.col;

    while (!inMap(player.row, player.col))
    {
        cout << "Invalid player position!" << endl;
        cout << "Please enter again:" << endl;
        cin >> player.row >> player.col;
    }
}

// ================================
// Printing
// ================================

// 判断玩家是否在某个格子上。
// 这个函数主要是为了让 printMap 更清楚。
bool playerAt(const Player &player, int row, int col)
{
    return player.row == row && player.col == col;
}

// 把一个 Tile 转换成终端显示用的字符串。
// 注意：这里不处理玩家。
// 玩家由 printMap 单独优先显示。
string tileToString(const Tile &tile)
{
    if (tile.type == TileType::Empty)
    {
        return "   ";
    }
    else if (tile.type == TileType::Wall)
    {
        return "###";
    }
    else if (tile.type == TileType::Key)
    {
        return " K ";
    }
    else if (tile.type == TileType::ExitLocked)
    {
        return "[X]";
    }
    else if (tile.type == TileType::ExitOpen)
    {
        return "[O]";
    }
    else if (tile.type == TileType::Crystal)
    {
        return " C ";
    }

    // Trap 的显示由 trap.state 决定。
    else if (tile.type == TileType::Trap)
    {
        if (tile.trap.state == TrapState::Hidden)
        {
            return " . ";
        }
        else if (tile.trap.state == TrapState::Armed)
        {
            return " ! ";
        }
        else if (tile.trap.state == TrapState::Fire)
        {
            return "***";
        }
        else
        {
            return "   ";
        }
    }

    // Guard 的显示由 guard.state 决定。
    else if (tile.type == TileType::Guard)
    {
        if (tile.guard.state == GuardState::Sleeping)
        {
            return "zzz";
        }
        else if (tile.guard.state == GuardState::Alert)
        {
            return " G ";
        }
        else if (tile.guard.state == GuardState::Tired)
        {
            return " g ";
        }
        else
        {
            return "   ";
        }
    }

    return "   ";
}

// 打印地图。
// 为了清楚，采用和 CS Flowers 类似的格子边框。
void printMap(const Grid &map, const Player &player)
{
    for (int row = 0; row < ROWS; row++)
    {
        // 打印横向边框
        for (int col = 0; col < COLS; col++)
        {
            cout << "+---";
        }
        cout << "+" << endl;

        // 打印每一格内容
        for (int col = 0; col < COLS; col++)
        {
            cout << "|";

            if (playerAt(player, row, col))
            {
                cout << " P ";
            }
            else
            {
                cout << tileToString(map[row][col]);
            }
        }
        cout << "|" << endl;
    }

    // 最后一行边框
    for (int col = 0; col < COLS; col++)
    {
        cout << "+---";
    }
    cout << "+" << endl;
}