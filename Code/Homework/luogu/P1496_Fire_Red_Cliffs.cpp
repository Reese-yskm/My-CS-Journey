/**
 * @file P1496_Fire_Red_Cliffs.cpp
 * @brief 洛谷 P1496 火烧赤壁 - 区间合并与离散化思想
 *
 * 【题目分析】
 * 1. 核心需求：给定多段起火区间 [a, b)，求被覆盖的总长度。
 * 2. 难点：坐标范围极大 ([-2^31, 2^31])，传统的数组下标无法表示。
 * 3. 约束：信息条数 N 较小 (2*10^4)。
 *
 * 【解法一：贪心区间合并 (你最初 AC 的方案)】
 * - 逻辑：按起点排序，维护当前正在燃烧的区间 [cur_st, cur_en]。
 * - 优点：时间 O(N log N)，空间 O(N)，逻辑直观。
 * - 坑点：初始化起点不能用 -1e9 等“假想最小值”，应直接取第一段的端点。
 *
 * 【解法二：离散化 Discretization (进阶方案)】
 * - 逻辑：将巨大的坐标映射为“排名”，把无限的数轴变成有限的“格点”。
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll; // 面对 2^31 这种逼近 int 极限的数，全线用 ll 护航

struct Segment
{
    ll st, en;
};

/**
 * 【重点讲解：什么是离散化？】
 *
 * 1. 为什么要离散化？
 *    当坐标范围是 40 亿，但实际有意义的点只有 4 万个时，数轴上绝大多数位置是“空的”。
 *    离散化就是把这 4 万个点“挤”在一起，去掉中间没意义的空隙。
 *
 * 2. 离散化的三步走模板：
 *    Step A: 收集所有端点 (起点和终点) 存入 vector。
 *    Step B: 排序 + 去重 (Unique)。得到一个有序的“骨架点”序列 coords。
 *    Step C: 映射。用二分查找 (lower_bound) 找到原坐标在 coords 里的下标。
 *
 * 3. 关键逻辑：点 vs 区间
 *    假设有序点为：-1, 1, 2, 5, 9, 11
 *    这些点构成了 5 个基础区间：[-1,1], [1,2], [2,5], [5,9], [9,11]
 *    我们标记的是“区间”而不是“点”。第 i 个区间对应原长度：coords[i+1] - coords[i]。
 */

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n) || n == 0)
    {
        cout << 0 << endl;
        return 0;
    }

    vector<Segment> segs(n);
    vector<ll> coords; // 存放所有的骨架点

    for (int i = 0; i < n; i++)
    {
        cin >> segs[i].st >> segs[i].en;
        coords.push_back(segs[i].st);
        coords.push_back(segs[i].en);
    }

    // --- A. 离散化预处理 ---
    sort(coords.begin(), coords.end());
    // unique 将重复元素移到末尾，erase 彻底删除它们
    coords.erase(unique(coords.begin(), coords.end()), coords.end());

    // --- B. 建立标记画板 ---
    // 如果有 K 个点，则产生 K-1 个小区间
    int K = coords.size();
    vector<bool> is_burning(K, false);

    // --- C. 映射并染色 ---
    for (int i = 0; i < n; i++)
    {
        // 找到起点和终点在“骨架数组”中的排名
        int s_idx = lower_bound(coords.begin(), coords.end(), segs[i].st) - coords.begin();
        int e_idx = lower_bound(coords.begin(), coords.end(), segs[i].en) - coords.begin();

        // 【翻译关键】：将原始区间覆盖转化为对“格子下标”的标记
        // 标记 [s_idx, e_idx) 范围内的基础区间正在燃烧
        for (int j = s_idx; j < e_idx; j++)
        {
            is_burning[j] = true;
        }
    }

    // --- D. 统计真相 ---
    ll total_len = 0;
    for (int i = 0; i < K - 1; i++)
    {
        if (is_burning[i])
        {
            // 如果第 i 个格子在烧，其真实长度就是两点坐标之差
            total_len += (coords[i + 1] - coords[i]);
        }
    }

    cout << total_len << endl;

    return 0;
}

/**
 * 【学习总结与避坑指南】 —— 整理自我的 RE 经历
 *
 * 1. 空间预估陷阱：
 *    - 错误做法：vector<bool> is(20005)。
 *    - 真相：N=20000 条信息有 40000 个端点。离散化后的数组大小必须对应端点的总数。
 *    - 启示：离散化后的标记数组大小应为 vector.size()。
 *
 * 2. 左右边界细节：
 *    - 本题是“左闭右开” [a, b)，所以标记区间时是从 s_idx 到 e_idx-1。
 *    - 如果标记到了 e_idx，就多算了一个区间。
 *
 * 3. 翻译能力的进化：
 *    通过离散化，你学会了把“数值本身”看作“地址”。
 *    这种思想在处理“矩形面积并”、“扫描线算法”中是唯一的通用解法。
 */