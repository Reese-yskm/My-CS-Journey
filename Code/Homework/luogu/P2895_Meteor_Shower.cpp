/**
 * @file P2895_Meteor_Shower.cpp
 * @brief 洛谷 P2895 [USACO08FEB] Meteor Shower S - 动态 BFS 建模
 *
 * 【题目核心】
 * 1. 在一个坐标系中，从 (0,0) 出发寻找最短路径。
 * 2. 障碍物（流星）是随时间变化的：格子在时刻 T 被摧毁后就不能进入。
 * 3. 目标不是固定坐标，而是“第一个不会被砸中的格子”。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // 为了使用 min

using namespace std;

// --- 常量与全局变量 ---
const int INF = 0x3f3f3f3f; // 用一个足够大的数表示“永远安全”
const int SIZE = 305;       // 坐标 0-300，开到 305 防止越界
int fire_map[SIZE][SIZE];   // 记录每个格子被摧毁的最早时间
bool visited[SIZE][SIZE];   // BFS 必不可少的防重标记

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

struct State
{
    int x, y, time;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. 初始化地图：假设所有格子初始都是永远安全的 (INF)
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fire_map[i][j] = INF;
        }
    }

    // 2. 预处理流星：记录每个格子被摧毁的【最早时间】
    int m;
    cin >> m;
    while (m--)
    {
        int x, y, t;
        cin >> x >> y >> t;
        // 更新中心点
        fire_map[x][y] = min(fire_map[x][y], t);
        // 更新周围四个邻居（注意边界检查）
        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < 302 && ny >= 0 && ny < 302)
            { // 稍微开大一点边界
                fire_map[nx][ny] = min(fire_map[nx][ny], t);
            }
        }
    }

    // --- 特殊判断：起点是否一出生就挂了 ---
    if (fire_map[0][0] == 0)
    {
        cout << -1;
        return 0;
    }

    // 3. 开始 BFS
    queue<State> q;
    q.push({0, 0, 0});
    visited[0][0] = true;

    while (!q.empty())
    {
        State cur = q.front();
        q.pop();

        // 【终点判断】：如果当前格子永远不会被流星砸中，说明安全了
        if (fire_map[cur.x][cur.y] == INF)
        {
            cout << cur.time;
            return 0;
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];
            int nt = cur.time + 1; // 下一格到达的时间

            // 【核心限制条件检查】：
            // a. 边界：坐标 >= 0
            // b. 没走过：!visited
            // c. 安全：到达的时间 nt 必须【小于】格子被摧毁的时间
            if (nx >= 0 && ny >= 0 && nx < 302 && ny < 302 && !visited[nx][ny] && nt < fire_map[nx][ny])
            {
                visited[nx][ny] = true;
                q.push({nx, ny, nt});
            }
        }
    }

    cout << -1; // 搜遍所有能走的地方都没找到安全区
    return 0;
}

/**
 * 【复盘与血泪教训】 —— 那些我踩过的坑
 *
 * 1. 数组初始化 (C++ 陷阱)：
 *    - 错误尝试：`int a[10] = {100}`。这只会把第一个数设为 100，其余是 0。
 *    - 正确做法：使用循环遍历，或 `memset`（仅限 0 或 -1），或 `fill` 函数。
 *
 * 2. 覆盖逻辑 (Min 的运用)：
 *    - 初次写错：`fire_map[x][y] = t;`
 *    - 现实：如果同一个格子被多个流星波及，它在【最早】那个流星降临时就坏了。
 *    - 修正：必须用 `min(fire_map[x][y], t)` 来更新地图。
 *
 * 3. 边界范围 (Boundary)：
 *    - 初次写错：坐标限制在 300 以内。
 *    - 现实：流星砸在 300，贝茜可以绕路走到 301, 302 等“世外桃源”。
 *    - 修正：数组和判断边界都要稍微留出“余裕”。
 *
 * 4. 时刻判断 (t vs t+1)：
 *    - 初次写错：`t < fire_map[nx][ny]`。
 *    - 现实：你现在的时刻是 t，移动到下一格是 t+1。你必须在 t+1 秒之前到达。
 *
 * 5. 起点安全 (The Beginning)：
 *    - 别忘了检查 (0,0) 是否在第 0 秒就被砸了。
 *
 * 【总结】
 * 这种题属于“带限制的最短路径”。关键在于把“环境限制”预处理成一个静态的“时刻表”，
 * 然后在 BFS 过程中，把时间作为状态的一部分同步移动。
 */