/**
 * @file P3884_Binary_Tree_LCA.cpp
 * @brief 洛谷 P3884 二叉树问题 - 综合应用 (BFS, 深度, 宽度, LCA)
 *
 * 【题目复盘】
 * 1. 深度：根节点到叶子的最长路径层数。
 * 2. 宽度：同一层节点数的最大值。
 * 3. 距离：从 x 到 y，向根走一步权值为 2，向叶子走一步权值为 1。
 *
 * 【核心建模：LCA (最近公共祖先)】
 * 树上任意两点 x, y 的路径必经过其 LCA。
 * 路径拆分：
 * - 上升段：x -> LCA (每步权值 2)
 * - 下降段：LCA -> y (每步权值 1)
 * 距离公式：Dist = (depth[x] - depth[LCA]) * 2 + (depth[y] - depth[LCA]) * 1
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// --- 全局变量 ---
int n, x, y;
vector<int> children[105]; // 存储子节点（下行）
int parent[105];           // 存储父节点（上行）
int depth[105];            // 每个节点的深度
int width_count[105];      // 每层节点的统计

struct Node
{
    int id;
    int d;
};

/**
 * @brief 寻找最近公共祖先 (LCA)
 * 算法逻辑：
 * 1. 从 x 节点开始，沿着 parent 一直往上爬到根节点，标记路径。
 * 2. 从 y 节点开始往上爬，遇到的第一个被标记过的点就是 LCA。
 */
int findLCA(int u, int v)
{
    bool marked[105] = {false};

    // 关键点：从节点自身开始爬，不能从 parent 开始，否则会漏掉“自身是祖先”的情况
    int temp = u;
    while (temp != 0)
    { // 根节点的 parent 为 0
        marked[temp] = true;
        temp = parent[temp];
    }

    temp = v;
    while (temp != 0)
    {
        if (marked[temp])
            return temp;
        temp = parent[temp];
    }
    return 1; // 默认根节点
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    // 1. 构建树结构
    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        children[u].push_back(v); // u 是 v 的父
        parent[v] = u;
    }
    cin >> x >> y;

    // 2. BFS 统计深度和宽度
    queue<Node> q;
    q.push({1, 1}); // 根节点为 1，深度起始为 1
    depth[1] = 1;

    int max_depth = 0;
    while (!q.empty())
    {
        Node cur = q.front();
        q.pop();

        int u = cur.id;
        int d = cur.d;
        depth[u] = d;

        // 更新最大深度
        max_depth = max(max_depth, d);
        // 统计该层宽度
        width_count[d]++;

        for (int v : children[u])
        {
            q.push({v, d + 1});
        }
    }

    // 统计最大宽度
    int max_width = 0;
    for (int i = 1; i <= max_depth; i++)
    {
        max_width = max(max_width, width_count[i]);
    }

    // 3. 计算 LCA 及加权距离
    int lca = findLCA(x, y);
    int dist = (depth[x] - depth[lca]) * 2 + (depth[y] - depth[lca]);

    // 输出结果
    cout << max_depth << endl;
    cout << max_width << endl;
    cout << dist << endl;

    return 0;
}

/**
 * 【关键核心知识点 & 注意事项】
 *
 * 1. BFS 的肌肉记忆：
 *    - 虽然你曾经写错很多次，但这次你独立写出了带 depth 的状态转移。
 *    - 记住：BFS 是“按层扩散”的，处理树的深度和宽度是它的拿手好戏。
 *
 * 2. LCA 的“自身包含”原则 (本题最大坑点)：
 *    - 找公共祖先时，如果写 `while(p[temp]!=0)`，会漏掉判断节点自己。
 *    - 修正：应写 `while(temp!=0)`。因为 LCA 可能是 x 或 y 其中的一个。
 *
 * 3. 树的表示法：
 *    - “双向奔赴”：用 vector 存子节点用于向下遍历，用数组存父节点用于向上攀爬。
 *    - 这种双向建模在处理树上路径问题时非常高效。
 *
 * 4. 距离建模：
 *    - 树上路径 = (x 到 LCA) + (LCA 到 y)。
 *    - 向根走一步权重为 2，向叶子走一步权重为 1。
 *    - 这个权重差异通过 (depth_diff * 2) 和 (depth_diff * 1) 完美体现。
 *
 * 5. 字典序与 sort：
 *    - 统计宽度时，你用了 sort 取最大值，这是一个稳妥的办法。
 *    - 在循环中动态更新 `max_width = max(...)` 则更节省空间和时间。
 */