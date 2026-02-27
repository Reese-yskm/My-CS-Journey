/**
 * @file Dijkstra_Optimized.cpp
 * @author Reese_fan (NJU-CS)
 * @brief Dijkstra 算法堆优化实现 - O(M log N) 复杂度
 * @date 2026-02-27
 *
 * [优化原理]
 * 1. 核心瓶颈：基础版每次寻找距离起点最近的点需要 O(N)。
 * 2. 解决方案：使用优先队列（小顶堆）维护所有未确定的点，将寻找最小值的时间降至 O(1)，维护时间降至 O(log N)。
 * 3. 适用场景：节点数 N 较大（10^5 级）且边数 M 较少的稀疏图。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// 使用 long long 防止在某些超长路径题目中发生 int 溢出
const long long INF = LLONG_MAX;

/**
 * @brief 堆优化 Dijkstra
 * @param n 节点数
 * @param start 起点
 * @param adj 邻接表 {邻居 v, 边权 w}
 */
void dijkstra_heap(int n, int start, const vector<vector<pair<int, int>>> &adj)
{
    // 1. 初始化
    vector<long long> dist(n + 1, INF);
    vector<bool> visited(n + 1, false);

    // 优先队列：{当前距离, 节点编号}
    // greater<pair<int, int>> 确保这是小顶堆（距离小的先弹出）
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty())
    {
        // 2. 取出当前堆顶（距离起点最近且未处理的点）
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // 【关键】由于一个点可能以不同的距离多次入堆，如果当前弹出的距离不是最新的，直接跳过
        if (d > dist[u])
            continue;

        // 或者使用你习惯的 visited 数组判断
        if (visited[u])
            continue;
        visited[u] = true;

        // 3. 遍历 u 的所有邻居
        for (const auto &edge : adj[u])
        {
            int v = edge.first;
            int weight = edge.second;

            // 松弛操作 (Relaxation)
            // 注意：这里是 dist[u] + weight，而不是 dist[weight]
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    // 4. 输出所有点的最短距离（示例输出到所有点）
    for (int i = 1; i <= n; i++)
    {
        cout << "Node " << i << ": ";
        if (dist[i] == INF)
            cout << "Unreachable" << endl;
        else
            cout << dist[i] << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m))
        return 0;

    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // 无向图
    }

    int start;
    cin >> start;

    dijkstra_heap(n, start, adj);

    return 0;
}