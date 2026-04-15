/**
 * @file Graph_Basics.cpp
 * @brief 图的遍历基础：DFS/BFS 与 逆向建模
 *
 * 【知识点 1：P5318 查找文献 - 基础遍历】
 * 1. 邻接表存储：vector<int> adj[N] 是最通用的方式。
 * 2. 排序要求：若要求“编号最小优先”，必须对每个节点的邻居进行 sort。
 * 3. 核心区别：图遍历必须带 visited 数组，且 BFS 建议“入队即标记”。
 *
 * 【知识点 2：P3916 图的遍历 - 逆向思维】
 * 1. 正向搜索的瓶颈：对每个点做 DFS 会导致 O(N*(N+M))，超时。
 * 2. 建模转换：
 *    - 原命题：从 u 出发能到达的最大编号 v。
 *    - 逆命题：从大编号 v 出发，反向能到达哪些点 u。
 * 3. 技巧：反向建图 (Reverse Graph)。
 *    - 如果存在边 u -> v，则建立边 v -> u。
 *    - 从 i = n 倒序循环到 1，若 i 未被访问，则从 i 开始 DFS。
 *    - 第一次访问到点 x 的大点 i，就是 x 能到达的最大编号。
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// --- 模拟 P3916 的反向建图核心代码 ---
vector<int> rev_adj[100005];
int ans[100005];

void reverse_dfs(int cur, int max_id)
{
    ans[cur] = max_id; // 染色：将当前能到达的点都标记为 max_id
    for (int next_node : rev_adj[cur])
    {
        if (!ans[next_node])
        { // 剪枝：已经被更大点标记过的不再进入
            reverse_dfs(next_node, max_id);
        }
    }
}

// --- 总结 ---
/**
 * 1. 图论题看到 N=10^5，一定要想办法把复杂度控制在 O(N+M)。
 * 2. 当“多对一”或“寻找最值”时，考虑反向建图。
 * 3. 排序顺序（正序/逆序）往往决定了贪心策略的正确性。
 */