#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// p 是价格表，n 是钢条总长度
int bottom_up_cut_rod(const vector<int> &p, int n)
{
    vector<int> r(n + 1, 0); // r[i] 存储长度为 i 的钢条的最大收益

    for (int j = 1; j <= n; j++)
    {
        int q = -1;
        // 尝试每一种切割位置 i
        for (int i = 1; i <= j; i++)
        {
            // 核心状态转移方程：
            // 长度 j 的最大收益 = max(当前最大收益, 长度 i 的价格 + 长度 j-i 的已求得最大收益)
            q = max(q, p[i] + r[j - i]);
        }
        r[j] = q; // 记下长度 j 的最优解
    }
    return r[n];
}

int main()
{
    // 价格表：下标为长度，值为价格 (第0位不用)
    vector<int> prices = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    int length = 10;

    printf("--- 1. Rod Cutting DP Test ---\n");
    int max_profit = bottom_up_cut_rod(prices, length);
    printf("Max profit for length %d is: %d\n", length, max_profit);

    return 0;
}