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
bool canPlaceWall(const Grid &map, const Player &player, int row, int col);
void placeWalls(Grid &map, const Player &player);
bool playerCanMoveTo(const Grid &map, int row, int col);
void movePlayer(Grid &map, Player &player, char command);
void commandLoop(Grid &map, Player &player);
bool canPlaceItem(const Grid &map, const Player &player, int row, int col);
void placeKeyAndExit(Grid &map, const Player &player);
void updateExit(Grid &map, const Player &player);
void handlePlayerTile(Grid &map, Player &player);
bool playerWon(const Grid &map, const Player &player);
bool endStep(const Grid &map, Player &player);
bool canPlaceTrap(const Grid &map, const Player &player, int row, int col);
void placeTraps(Grid &map, const Player &player);
void triggerTrap(Grid &map, const Player &player);
void updateTrapStates(Grid &map);
bool playerOnFire(const Grid &map, const Player &player);
void upkeepStep(Grid &map, Player &player);
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
    // 放置墙
    placeWalls(map, player);
    // 放置钥匙
    placeKeyAndExit(map, player);
    // 放置陷阱
    placeTraps(map, player);
    // 主循环
    commandLoop(map, player);
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

// ================================
// My funtion
// ================================

bool canPlaceWall(const Grid &map, const Player &player, int row, int col)
{
    if (!inMap(row, col))
    {
        return false;
    }
    if (row == player.row && col == player.col)
    {
        return false;
    }
    if (map[row][col].type != TileType::Empty)
    {
        return false;
    }
    return true;
}
void placeWalls(Grid &map, const Player &player)
{
    int count;
    cout << "How many walls:";
    cin >> count;
    while (count--)
    {
        int wall_row, wall_col;
        cin >> wall_row >> wall_col;
        if (canPlaceWall(map, player, wall_row, wall_col))
        {
            map[wall_row][wall_col].type = TileType::Wall;
            cout << "Place successfully!\n";
        }
        else
        {
            cout << "Unvalid positon!\n";
        }
        printMap(map, player);
    }
}
bool playerCanMoveTo(const Grid &map, int row, int col)
{
    if (!inMap(row, col))
    {
        return false;
    }
    if (map[row][col].type == TileType::Wall)
    {
        return false;
    }
    return true;
}
void movePlayer(Grid &map, Player &player, char command)
{
    int finalRow = player.row;
    int finalCol = player.col;
    if (command == 'w')
    {
        finalRow--;
    }
    else if (command == 'a')
    {
        finalCol--;
    }
    else if (command == 's')
    {
        finalRow++;
    }
    else if (command == 'd')
    {
        finalCol++;
    }
    if (playerCanMoveTo(map, finalRow, finalCol))
    {
        player.row = finalRow;
        player.col = finalCol;
        handlePlayerTile(map, player);
        triggerTrap(map, player);
    }
    else
    {
        cout << "Unvalid move!\n";
    }
}
void commandLoop(Grid &map, Player &player)
{

    while (true)
    {
        cout << "Please enter the diretion:";
        char command;
        cin >> command;
        if (command == 'q')
        {
            cout << "Game quit!\n";
            break;
        }
        movePlayer(map, player, command);
        upkeepStep(map, player);
        printMap(map, player);
        if (endStep(map, player))
        {
            break;
        }
    }
}
bool canPlaceItem(const Grid &map, const Player &player, int row, int col)
{
    if (!inMap(row, col))
    {
        return false;
    }
    if (row == player.row && col == player.col)
    {
        return false;
    }
    if (map[row][col].type != TileType::Empty)
    {
        return false;
    }
    return true;
}
void placeKeyAndExit(Grid &map, const Player &player)
{
    cout << "Please enter key potsition:";
    int key_row, key_col;
    cin >> key_row >> key_col;
    while (!canPlaceItem(map, player, key_row, key_col))
    {
        cout << "Unvalid place!Please palce the key again:";
        cin >> key_row >> key_col;
    }
    cout << "Place successfully!\n";
    map[key_row][key_col].type = TileType::Key;
    printMap(map, player);

    cout << "Please enter exit potsition:";
    int exit_row, exit_col;
    cin >> exit_row >> exit_col;
    while (!canPlaceItem(map, player, exit_row, exit_col))
    {
        cout << "Unvalid place!Please palce the exit again:";
        cin >> exit_row >> exit_col;
    }
    cout << "Place successfully!\n";
    map[exit_row][exit_col].type = TileType::ExitLocked;
    printMap(map, player);
}
void updateExit(Grid &map, const Player &player)
{
    if (!player.hasKey)
    {
        return;
    }
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (map[r][c].type == TileType::ExitLocked)
            {
                map[r][c].type = TileType::ExitOpen;
            }
        }
    }
}
void handlePlayerTile(Grid &map, Player &player)
{
    if (map[player.row][player.col].type != TileType::Key)
    {
        return;
    }
    map[player.row][player.col].type = TileType::Empty;
    player.hasKey = true;
    updateExit(map, player);
}
bool playerWon(const Grid &map, const Player &player)
{
    return map[player.row][player.col].type == TileType::ExitOpen;
}
bool endStep(const Grid &map, Player &player)
{
    if (playerWon(map, player))
    {
        cout << "You escaped the dungeon!";
        return true;
    }
    if (playerOnFire(map, player))
    {
        cout << "You are killed by fire!";
        return true;
    }
    return false;
}
bool canPlaceTrap(const Grid &map, const Player &player, int row, int col)
{
    if (!inMap(row, col))
    {
        return false;
    }
    if (row == player.row && col == player.col)
    {
        return false;
    }
    if (map[row][col].type != TileType::Empty)
    {
        return false;
    }
    return true;
}
void placeTraps(Grid &map, const Player &player)
{
    int count;
    cout << "How many traps:";
    cin >> count;
    while (count--)
    {
        int trap_row, trap_col;
        cin >> trap_row >> trap_col;
        if (canPlaceTrap(map, player, trap_row, trap_col))
        {
            map[trap_row][trap_col].type = TileType::Trap;
            map[trap_row][trap_col].trap.state = TrapState::Hidden;
            cout << "Place successfully!\n";
        }
        else
        {
            cout << "Unvalid positon!\n";
        }
        printMap(map, player);
    }
}
void triggerTrap(Grid &map, const Player &player)
{
    if (map[player.row][player.col].type != TileType::Trap)
    {
        return;
    }
    if (map[player.row][player.col].trap.state == TrapState::Hidden)
    {
        map[player.row][player.col].trap.state = TrapState::Armed;
        map[player.row][player.col].trap.newlyArmed = true;
    }
}
void updateTrapStates(Grid &map)
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (map[r][c].type == TileType::Trap)
            {
                if (map[r][c].trap.state == TrapState::Armed)
                {
                    if (map[r][c].trap.newlyArmed == false)
                    {
                        map[r][c].trap.state = TrapState::Fire;
                    }
                    else
                    {
                        map[r][c].trap.newlyArmed = false;
                    }
                }
                else if (map[r][c].trap.state == TrapState::Fire)
                {
                    map[r][c].trap.state = TrapState::None;
                    map[r][c].type = TileType::Empty;
                }
            }
        }
    }
}
bool playerOnFire(const Grid &map, const Player &player)
{
    if (map[player.row][player.col].trap.state != TrapState::Fire)
    {
        return false;
    }
    return true;
}
void upkeepStep(Grid &map, Player &player)
{
    if (playerOnFire(map, player))
    {
        player.alive = false;
    }
    updateTrapStates(map);
}