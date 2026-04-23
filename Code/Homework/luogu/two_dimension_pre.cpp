/**
 * @file 2D_Prefix_Sum_Note.cpp
 * @brief 二维前缀和算法模板与应用 - 洛谷 P2004 领地选择
 *
 * 【核心思想：容斥原理】
 * 二维前缀和用于快速计算矩阵中任意子矩形的数值总和。
 *
 * 1. 预处理：定义 S[i][j] 为从 (1,1) 到 (i,j) 的矩形内所有元素的和。
 *    递推公式：S[i][j] = val[i][j] + S[i-1][j] + S[i][j-1] - S[i-1][j-1]
 *    (含义：当前格 + 上方矩形 + 左方矩形 - 重叠被加了两次的左上方矩形)
 *
 * 2. 查询：计算左上角 (x1, y1) 到右下角 (x2, y2) 的子矩形和。
 *    查询公式：Sum = S[x2][y2] - S[x1-1][y2] - S[x2][y1-1] + S[x1-1][y1-1]
 *    (含义：总矩形 - 上方多余 - 左方多余 + 扣了两次被多扣掉的左上方矩形)
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 使用 long long 防止大矩阵求和溢出
long long s[1005][1005];

int main()
{
    // 性能优化：大数据量下必备
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, c;
    if (!(cin >> n >> m >> c))
        return 0;

    // 1. 预处理二维前缀和
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            int val;
            cin >> val;
            // 套用预处理公式
            s[i][j] = val + s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1];
        }
    }

    long long max_val = -2e18; // 初始化为极小值，应对地图全为负数的情况
    int ansX = 1, ansY = 1;

    // 2. 遍历所有可能的左上角坐标 (i, j)
    // 注意边界：左上角为 (i, j)，则右下角为 (i+c-1, j+c-1)
    // 所以 i 的上限是 n-c+1，j 的上限是 m-c+1
    for (int i = 1; i <= n - c + 1; i++)
    {
        for (int j = 1; j <= m - c + 1; j++)
        {
            int x1 = i, y1 = j;
            int x2 = i + c - 1;
            int y2 = j + c - 1;

            // 套用查询公式，O(1) 得到当前正方形的价值总和
            long long current_sum = s[x2][y2] - s[x1 - 1][y2] - s[x2][y1 - 1] + s[x1 - 1][y1 - 1];

            if (current_sum > max_val)
            {
                max_val = current_sum;
                ansX = i;
                ansY = j;
            }
        }
    }

    // 3. 输出最优解左上角坐标
    cout << ansX << " " << ansY << endl;

    return 0;
}

/**
 * 【避坑要点与翻译心得】
 *
 * 1. 坐标偏移 (Off-by-one Error)：
 *    这是最容易错的地方。如果左上角是 (i, j)，长度为 C，
 *    右下角不是 (i+C, j+C)，而是 (i+C-1, j+C-1)。
 *
 * 2. 内存布局：
 *    对于 1000x1000 的矩阵，尽量使用全局数组或动态 vector。
 *    s[i][j] 中 i 对应行(N/高)，j 对应列(M/宽)。
 *
 * 3. 适用场景：
 *    只要题目涉及“子矩阵求和”、“静态区域统计”，二维前缀和就是首选。
 *    如果是“动态修改矩形数值，再查询和”，则需要“二维树状数组”或“二维线段树”。
 */