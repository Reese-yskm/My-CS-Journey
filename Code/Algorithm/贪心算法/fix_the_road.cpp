#include <iostream>
#include <vector>

using namespace std;

/**
 * 【算法导论：第16章 贪心算法 - 铺设道路笔记】
 *
 * 1. 题目模型：
 *    给定一个高度序列，每次可以选一个不含 0 的区间 [L, R] 全体减 1，求减到全 0 的最少次数。
 *
 * 2. 你的分治直觉 (Divide and Conquer)：
 *    - 每次找到区间最小值 min_h，全体减去 min_h 次，区间会被 0 分隔。
 *    - 递归处理被分隔出的子区间。
 *    - 复杂度：最坏 O(n^2)，平均 O(n log n)。虽然正确，但在 n=10^5 时会超时。
 *
 * 3. 贪心策略 (Greedy)：
 *    - 我们从左向右看，维护一个变量 `last_depth`（前一个区域的深度）。
 *    - 如果 `current_depth > last_depth`：
 *        说明前一个区域的填充操作无法完全覆盖当前区域，
 *        必须额外增加 `current_depth - last_depth` 次操作。
 *    - 如果 `current_depth <= last_depth`：
 *        说明在填平前一个区域时，可以顺便把当前区域填平，不需要增加次数。
 *
 * 4. 差分视角 (Difference Array)：
 *    - 令差分数组 B[i] = A[i] - A[i-1]。
 *    - 每次区间 [L, R] 减 1，对应差分数组：B[L] 减 1，B[R+1] 加 1。
 *    - 目标是把所有 B[i] 变为 0。
 *    - 最少步数 = 差分数组中所有正数项的和（因为每个正数脉冲都必须由一个区间的开头来消除）。
 */

int main()
{
    // 提高 I/O 效率
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    if (!(cin >> n))
        return 0;

    long long total_days = 0; // 结果可能超过 int (10^5 * 10^4 = 10^9，接近上限，用 long long 保险)
    int last_depth = 0;       // 初始地面高度为 0

    for (int i = 0; i < n; i++)
    {
        int current_depth;
        cin >> current_depth;

        // 【贪心选择】
        // 只有当当前坑比前一个深时，才产生“无法被顺便填平”的增量代价
        if (current_depth > last_depth)
        {
            total_days += (current_depth - last_depth);
        }

        // 更新“前一个深度”
        last_depth = current_depth;
    }

    cout << total_days << endl;

    return 0;
}

/**
 * 【助教深度感悟】
 * 1. 维度转换：这道题最天才的地方在于把“区间操作”转换成了“相邻项差值”。
 * 2. 局部与整体：
 *    分治法是自顶向下的“上帝视角”（看全局最小值）；
 *    贪心法是自左向右的“凡人视角”（只看当前和前一个）。
 *    在数学上，凡人每一步的最优决策累加起来，恰好等于上帝的全局最优解。
 * 3. 关联题目：
 *    - 洛谷 P1969 [NOIP2013] 积木大赛（完全相同的题）。
 *    - 差分入门题：给定区间加 x，求最终数组。
 */