/**
 * @file P1443_KnightTour.cpp
 * @author Reese_fan (NJU-CS)
 * @brief 洛谷 P1443 马的遍历 - 经典广度优先搜索 (BFS)
 * @date 2026-02-25
 *
 * [算法感悟]
 * 1. 坚持的力量：连续多日的独立编码，让逻辑思维从迷茫变得清晰，这种成就感远超游戏。
 * 2. BFS 的本质：利用队列实现层级扩散，确保在无权图中第一次到达某个点时走的一定是最短路径。
 * 3. 坐标处理：读入即对齐 (x--, y--) 是避免 Off-by-one error 的关键。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> // 用于格式化输出

using namespace std;

// 定义坐标结构体
struct Pos
{
    int x, y;
};

int main()
{
    // 快速输入输出优化
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, sx, sy;
    if (!(cin >> n >> m >> sx >> sy))
        return 0;

    // 将 1-based 坐标转换为 0-based
    sx--;
    sy--;

    // 马的 8 个跳跃方向
    const int dx[] = {1, 1, 2, 2, -1, -1, -2, -2};
    const int dy[] = {2, -2, 1, -1, 2, -2, 1, -1};

    // dist 数组初始化为 -1，表示尚未到达
    // 使用 vector 的构造函数一次性初始化，更优雅
    vector<vector<int>> dist(n, vector<int>(m, -1));

    // 起点初始化
    queue<Pos> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});

    // 开始 BFS 涟漪搜索
    while (!q.empty())
    {
        Pos curr = q.front();
        q.pop();

        for (int i = 0; i < 8; i++)
        {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];

            // 边界检查及是否访问检查
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && dist[nx][ny] == -1)
            {
                dist[nx][ny] = dist[curr.x][curr.y] + 1;
                q.push({nx, ny});
            }
        }
    }

    // 格式化输出结果
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 使用 left 和 setw(5) 实现左对齐，占 5 个字符位
            cout << left << setw(5) << dist[i][j];
        }
        cout << "\n"; // 使用 \n 代替 endl 提高输出效率
    }

    return 0;
}