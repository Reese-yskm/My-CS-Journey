/**
 * @file P1825_Corn_Maze.cpp
 * @brief 洛谷 P1825 [USACO11OPEN] Corn Maze S - 带传送门的 BFS
 *
 * 【题目核心】
 * 1. 传统迷宫 BFS 的变种：每一步代价为 1。
 * 2. 特殊机制：字母 A-Z 是双向传送门，踩上去【必须】立即传送到另一端，且传送代价为 0。
 * 3. 目标：从 '@' 到 '=' 的最短时间。
 *
 * 【建模难点：传送门预处理】
 * 我们不能在搜索过程中临时去找传送门的另一端。
 * 方案：使用结构体数组 portal[26][2] 记录每个字母出现的两个坐标。
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// --- 数据结构定义 ---
struct Point
{
    int x, y;
};

struct State
{
    int x, y, time;
};

// 全局变量
int n, m;
char grid[305][305];
bool visited[305][305];
Point portal[26][2]; // 存储 26 种传送门的坐标对
int portal_cnt[26];  // 计数器，记录每个字母找到了几个点（0, 1, 2）

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. 读取地图并进行【关键预处理】
    cin >> n >> m;
    int sx, sy;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> grid[i][j];
            if (grid[i][j] == '@')
            {
                sx = i;
                sy = j; // 记录起点
            }
            else if (grid[i][j] >= 'A' && grid[i][j] <= 'Z')
            {
                // 预处理传送门坐标
                int id = grid[i][j] - 'A';
                portal[id][portal_cnt[id]] = {i, j};
                portal_cnt[id]++;
            }
        }
    }

    // 2. 标准 BFS 流程
    queue<State> q;
    q.push({sx, sy, 0});
    visited[sx][sy] = true;

    while (!q.empty())
    {
        State cur = q.front();
        q.pop();

        // 终点判断：到达 '=' 立即输出
        if (grid[cur.x][cur.y] == '=')
        {
            cout << cur.time;
            return 0;
        }

        // 尝试四个方向扩散
        for (int i = 0; i < 4; i++)
        {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            // A. 基础检查：边界 + 不是墙
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] != '#')
            {

                int tx = nx, ty = ny; // 最终落脚点

                // B. 【核心逻辑】：如果是传送门，立即发生坐标突变
                if (grid[nx][ny] >= 'A' && grid[nx][ny] <= 'Z')
                {
                    int id = grid[nx][ny] - 'A';
                    // 找到当前踩到的是哪一端，就跳到另一端
                    if (nx == portal[id][0].x && ny == portal[id][0].y)
                    {
                        tx = portal[id][1].x;
                        ty = portal[id][1].y;
                    }
                    else
                    {
                        tx = portal[id][0].x;
                        ty = portal[id][0].y;
                    }
                }

                // C. 检查最终落脚点是否访问过
                // 注意：这里标记和判断的必须是【突变后】的坐标 (tx, ty)
                if (!visited[tx][ty])
                {
                    visited[tx][ty] = true;
                    q.push({tx, ty, cur.time + 1});
                }
            }
        }
    }

    return 0;
}

/**
 * 【复盘笔记：我的易错点】
 *
 * 1. 传送门判断逻辑 (XY 同时判断)：
 *    - 错误：if (nx == portal[id][0].x)
 *    - 修正：必须 if (nx == portal[id][0].x && ny == portal[id][0].y)。
 *      因为两个传送门可能处于同一行或同一列，只判一个坐标会产生误判。
 *
 * 2. 传送逻辑的顺序：
 *    - 贝茜是“踩上去”就瞬间传送。
 *    - 逻辑：计算邻居(nx, ny) -> 发现是字母 -> 变换坐标为(tx, ty) -> 标记已访问并入队。
 *
 * 3. 关于 Visited 的标记时机：
 *    - 永远记住【入队即标记】原则。
 *    - 标记的对象应该是你这一步“最终停下来”的位置。
 *
 * 4. 连续传送的误区：
 *    - 题目说“必须移动至少一步才能再次传送”，这在 BFS 框架下天然成立。
 *    - 因为你传送到了 (tx, ty)，你的下一步必须通过 `for (dx, dy)` 扩散出去。
 *
 * 5. 迷宫终点判断：
 *    - 在这题中，终点判断放在 while 刚开始取队首时最稳健，因为传送门可能会把你直接扔到出口旁边。
 */