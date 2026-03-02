/**
 * @file P1789_Mc_Survival.cpp
 * @author Reese_fan (NJU-CS)
 * @brief 洛谷 P1789 插火把 - 模拟建模与偏移数组优化
 * @date 2026-03-02
 *
 * [算法感悟：思维的进化]
 * 1. 偏移数组的普适性：
 *    过去认为 dx[], dy[] 仅用于 4 连通或 8 连通。通过此题意识到，
 *    任何固定的几何形状（如火把的 13 点钻石形）都可以抽象为相对于中心点的坐标偏移集合。
 *    这种“数据驱动”的思想能极大简化复杂的边界逻辑判断。
 *
 * 2. 逻辑与数据分离：
 *    将图形的坐标点存入数组（数据），使用统一的循环进行合法性检查和填充（逻辑）。
 *    这不仅减少了硬编码（Hard-coding）带来的 Bug，也增强了代码的可维护性。
 *
 * 3. 辅助函数的价值：
 *    将边界检查 `x >= 0 && x < n` 封装为 `isValid()` 函数，
 *    使核心业务代码更清爽，读起来更接近自然语言。
 */

#include <iostream>
#include <vector>

using namespace std;

// 1. 全局定义偏移量，这就是“数据”
// 火把照亮的 12 个相对位置（不含中心）
const int torch_dx[] = {1, 2, -1, -2, 0, 0, 0, 0, 1, 1, -1, -1};
const int torch_dy[] = {0, 0, 0, 0, 1, 2, -1, -2, 1, -1, 1, -1};

// 2. 边界检查辅助函数
bool isValid(int x, int y, int n)
{
    return x >= 0 && x < n && y >= 0 && y < n;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<bool>> board(n, vector<bool>(n, false));
    while (m--)
    {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        board[x][y] = true;
        for (int i = 0; i < 12; i++)
        {
            int nx = x + torch_dx[i];
            int ny = y + torch_dy[i];
            if (isValid(nx, ny, n))
            {
                board[nx][ny] = true;
            }
        }
    }
    while (k--)
    {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        board[x][y] = true;
        for (int i = x - 2; i <= x + 2; i++)
        {
            for (int j = y - 2; j <= y + 2; j++)
            {
                if (isValid(i, j, n))
                {
                    board[i][j] = true;
                }
            }
        }
    }
    int res = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (board[i][j] == false)
            {
                res++;
            }
        }
    }
    cout << res;
}