/**
 * @file: P1162_Coloring_Flood_Fill.cpp
 * @brief: 洛谷 P1162 填涂颜色 - 漫水填充算法 (Flood Fill) 与 逆向思维
 * @author: [Your Name]
 * @date: 2026-04-01
 *
 * [题目核心]
 * 在一个 01 方阵中，寻找由 1 组成的闭合圈内部的 0，并将其全部替换为 2。
 * 闭合圈可能是任意形状。
 *
 *[思维升维：从正向判定到逆向排除]
 * 1. 为什么“射线探测”失效？
 *    闭合圈可能是凹多边形甚至迷宫形状，一个点向四个方向发射射线都被挡住，
 *    并不代表它在圈内（它可以通过绕弯走出去）。连通性问题不能用直线思维。
 * 2. 逆向思维（外围倒水法）：
 *    与其去证明哪些 0 在圈内，不如去找哪些 0 在圈外。
 *    将 1 视为墙壁。我们从地图最外围（四条边）的所有 0 开始“倒水”(DFS)。
 *    水能流到的 0，绝对在圈外；水流不到的 0，就是被 1 完美包围的圈内点。
 *
 * [算法选型：Flood Fill (漫水填充)]
 * - 它是 DFS/BFS 的一种特殊应用，常用于图形学中的“油漆桶”工具、扫雷游戏展开区块等。
 * - 特点：不需要回溯！因为我们只关心“连通块”的归属，不关心具体走哪条路径。
 */

#include <iostream>
#include <vector>

using namespace std;

int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};
int n;

/**
 * 漫水填充 DFS
 * @param a 引用传递二维矩阵，极大地节约内存与时间
 * @param x, y 当前坐标
 */
void dfs(vector<vector<int>> &a, int x, int y)
{
    // 递归边界：遇到墙壁(1) 或者 已经淹没过的水域(3) 就停止
    if (a[x][y] == 1 || a[x][y] == 3)
    {
        return;
    }

    // 染色：标记为被水淹没
    a[x][y] = 3;

    // 探究四个方向
    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        // 边界保护
        if (nx >= 0 && nx < n && ny >= 0 && ny < n)
        {
            dfs(a, nx, ny);
        }
    }
}

int main()
{
    // 优化 IO 速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n))
        return 0;

    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }

    // [边缘启动]：从四条边界的 0 开始注入水 (DFS)
    for (int i = 0; i < n; i++)
    {
        if (a[0][i] == 0)
            dfs(a, 0, i); // 第一行
        if (a[i][0] == 0)
            dfs(a, i, 0); // 第一列
        if (a[n - 1][i] == 0)
            dfs(a, n - 1, i); // 最后一行
        if (a[i][n - 1] == 0)
            dfs(a, i, n - 1); // 最后一列
    }

    // [全图清算]：映射输出
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (a[i][j] == 3)
            {
                // 外围的水，恢复为空地 0
                cout << 0 << " ";
            }
            else if (a[i][j] == 0)
            {
                // 没被水淹没的空地，说明在圈内，变成 2
                cout << 2 << " ";
            }
            else
            {
                // 墙壁 1 保持不变
                cout << a[i][j] << " ";
            }
        }
        cout << "\n";
    }

    return 0;
}

/**
 * [收获与感悟]
 * - 算法不仅仅是代码的堆砌，更是一种生活中的物理模型映射。
 * - 连通性（曲折的） vs 可视性（直线的）：连通性问题必须用搜索(DFS/BFS)图遍历，直线问题用射线倍乘。
 * - 传引用的重要性：在 C++ 递归中，遇到大型容器 (如 vector)，切记加上 &，这是防止爆内存的核心细节。
 */