/**
 * @file Dijkstra_Basic.cpp
 * @author Reese_fan (NJU-CS)
 * @brief Dijkstra 算法基础实现 - O(N^2) 复杂度
 * @date 2026-02-27
 *
 * [算法原理]
 * 1. 适用于带权重的单源最短路径问题（且权重不能为负）。
 * 2. 贪心策略：每次从未确定最短路的点中，选择一个距离起点最近的点。
 * 3. 松弛操作：以该点为中转站，更新其邻居到起点的最短距离。
 *
 * [复杂度分析]
 * 时间复杂度：O(N^2)，适用于节点数 N 在 5000 以内的稠密图。
 * 空间复杂度：O(N + E)，使用邻接表存储。
 */

#include <iostream>
#include <vector>
#include <climits> // 必须包含，用于 INT_MAX

using namespace std;

/**
 * @brief 执行 Dijkstra 算法计算最短路径
 * @param n 节点总数
 * @param start 起点编号
 * @param adj 邻接表
 * @return vector<int> 各点到起点的最短距离
 */
vector<int> dijkstra_basic(int n, int start, const vector<vector<pair<int, int>>> &adj)
{
    // 1. 初始化：所有距离为无穷大，所有点未访问
    vector<int> dist(n + 1, INT_MAX);
    vector<bool> visited(n + 1, false);

    dist[start] = 0;

    // 2. 核心循环：执行 n 次，每次确定一个点的最短路径
    for (int i = 1; i <= n; i++)
    {
        // --- 步骤 A: 寻找当前未访问点中距离起点最近的 ---
        int u = -1;
        int min_dist = INT_MAX;
        for (int j = 1; j <= n; j++)
        {
            if (!visited[j] && dist[j] < min_dist)
            {
                min_dist = dist[j];
                u = j;
            }
        }

        // 如果找不到可达的点（剩下的点与起点不连通），提前结束
        if (u == -1)
            break;

        // --- 步骤 B: 标记该点为已访问（固定最短路） ---
        visited[u] = true;

        // --- 步骤 C: 以该点为桥梁，尝试更新（松弛）其所有邻居的距离 ---
        for (const auto &edge : adj[u])
        {
            int v = edge.first;
            int weight = edge.second;

            // 只有当 u 可达且通过 u 到达 v 的距离更短时，才更新
            // 注意：dist[u] 已通过 u != -1 保证不是 INT_MAX
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
            }
        }
    }
    return dist;
}

int main()
{
    // 快速输入输出优化
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m))
        return 0;

    // 构建邻接表
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // 无向图需加两次
    }

    int start, des;
    cin >> start >> des;

    // 执行算法
    vector<int> result = dijkstra_basic(n, start, adj);

    // 输出结果
    if (result[des] == INT_MAX)
    {
        cout << "There is no path from " << start << " to " << des << "." << endl;
    }
    else
    {
        cout << "The min distance from " << start << " to " << des << " is : " << result[des] << endl;
    }

    return 0;
}