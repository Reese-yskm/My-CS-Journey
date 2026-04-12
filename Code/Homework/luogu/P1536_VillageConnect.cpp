/**
 * @file P1536_VillageConnect.cpp
 * @brief 洛谷 P1536 村村通 - 并查集 (DSU/Union-Find)
 *
 * 【题目描述】
 * 给出 n 个村庄和 m 条现有道路。求最少还需要建多少条路，使所有村庄连通。
 *
 * 【核心建模】
 * 1. 连通性问题：两个村庄只要在一个集合内，就是连通的。
 * 2. 并查集作用：通过“合并”操作处理现有道路，通过“查询”操作统计最终剩余的独立集合数。
 * 3. 结论：如果最终剩下 k 个独立的集合（连通分量），则需要 k-1 条路将它们连起来。
 *
 * 【并查集三大标准件】
 */

#include <iostream>
#include <vector>

using namespace std;

// --- 并查集标准函数 ---

/**
 * @brief 初始化：每个人最初都是自己的老大
 */
void init(vector<int> &fa, int n)
{
    for (int i = 1; i <= n; i++)
        fa[i] = i;
}

/**
 * @brief 查询 + 路径压缩 (Path Compression)
 * 这是并查集最高效的精华。在查询过程中，直接将路径上所有节点的父节点改为根节点。
 * 时间复杂度接近 O(1)。
 */
int find(vector<int> &fa, int a)
{
    if (fa[a] == a)
        return a;
    return fa[a] = find(fa, fa[a]); // 递归并赋值，实现路径压缩
}

/**
 * @brief 合并 (Union)
 * 注意：必须是“首领认首领为老大”。
 */
void uni(vector<int> &fa, int u, int v)
{
    int rootU = find(fa, u);
    int rootV = find(fa, v);
    if (rootU != rootV)
    {
        fa[rootV] = rootU; // 两个集合的首领合并
    }
}

int main()
{
    // 优化输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    // 多组数据处理：当 n 不为 0 时继续执行
    while (cin >> n && n != 0)
    {
        cin >> m;

        vector<int> fa(n + 1);
        init(fa, n);

        // 读入已有的道路并进行合并
        for (int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            uni(fa, u, v);
        }

        // 统计最终剩下的独立集合数量
        int k = 0;
        for (int i = 1; i <= n; i++)
        {
            // 只要 fa[i] == i，说明 i 是一派之首，代表一个独立的连通块
            if (fa[i] == i)
                k++;
        }

        // 输出结果：k 个块需要 k-1 条线连接
        cout << k - 1 << endl;
    }

    return 0;
}

/**
 * 【关键注意事项】
 *
 * 1. 回忆 Kruskal：
 *    - 最小生成树算法中判断是否会形成环，就是看边的两个端点是否已经在同一个并查集里。
 *
 * 2. 传引用 (&)：
 *    - C++ 中处理 vector 等容器，若不加 &，函数操作的是副本。
 *
 * 3. 多组数据重置：
 *    - 每一组测试数据开始前，fa 数组必须重新 init。
 *
 * 4. 统计技巧：
 *    - `fa[i] == i` 的节点个数 = 连通块个数。
 *    - 这个技巧在处理图中“有多少个独立的省份/群体”时非常通用。
 */