/**
 * @file P1318_TrappingRainWater.cpp
 * @author YourName (NJU-CS)
 * @brief 洛谷 P1318 积水面积 - 经典双向预处理解法 (O(n))
 * @date 2023-XX-XX
 *
 * [题目描述]
 * 给定一组柱子高度，求这些柱子之间能积水的总面积。
 *
 * [算法思路 - 木桶原理/双向最值预处理]
 * 1. 局部思维：每一列积水的高度取决于其左侧所有柱子的最大值 (L_max)
 *    和右侧所有柱子的最大值 (R_max) 中的较小者。
 * 2. 核心公式：Area[i] = max(0, min(L_max[i], R_max[i]) - height[i])
 * 3. 预处理优化：通过两次扫描分别计算 Prefix Max 和 Suffix Max，将复杂度从 O(n^2) 降至 O(n)。
 *
 * [编程心法 - 定义的重要性]
 * 关键在于 L_max[i] 的定义：包含当前柱子 i 本身。
 * - 若定义包含 i：L_max[i] = max(L_max[i-1], height[i])。计算时无需特判，
 *   若自己最高，则 min(L, R) - height = 0，逻辑完美自洽。
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief 计算积水总面积
 * @param n 柱子数量
 * @param height 柱子高度向量
 * @return long long 总积水面积 (注意数据范围，建议使用 long long)
 */
long long solve_trapping_water(int n, const vector<int> &height)
{
    if (n <= 2)
        return 0; // 首尾为0且长度小于3不可能积水

    // L_max[i]: 索引 i 及其左边所有柱子的最高高度
    // R_max[i]: 索引 i 及其右边所有柱子的最高高度
    vector<int> L_max(n), R_max(n);

    // 1. 前缀扫描：计算左侧最大值 (Prefix Maximum)
    L_max[0] = height[0];
    for (int i = 1; i < n; ++i)
    {
        L_max[i] = max(L_max[i - 1], height[i]);
    }

    // 2. 后缀扫描：计算右侧最大值 (Suffix Maximum)
    R_max[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; --i)
    {
        R_max[i] = max(R_max[i + 1], height[i]);
    }

    // 3. 累加积水：利用木桶短板原理
    long long total_area = 0;
    for (int i = 0; i < n; ++i)
    {
        // 当前列的水位由两边最高者的较小值决定
        int water_level = min(L_max[i], R_max[i]);
        // 只有水位高于柱子本身高度时才有积水
        // 因为定义包含了本身，所以此处 height[i] 不可能大于 water_level
        total_area += (water_level - height[i]);
    }

    return total_area;
}

int main()
{
    // 性能优化：关闭同步，解除 cin/cout 绑定
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n))
        return 0;

    vector<int> height(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> height[i];
    }

    cout << solve_trapping_water(n, height) << endl;

    return 0;
}

/**
 * [复杂度分析]
 * - 时间复杂度: O(N)，经历了三次线性扫描。
 * - 空间复杂度: O(N)，使用了两个辅助数组存储左右最大值。
 *
 * [面试扩展]
 * 该题在 LeetCode 42. Trapping Rain Water 是 Hard 级别。
 * 进阶优化：可以使用“双指针”法将空间复杂度进一步降至 O(1)。
 */