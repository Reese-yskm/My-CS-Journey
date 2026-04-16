/**
 * @file P1807_Longest_Path.cpp
 * @brief 洛谷 P1807 最长路 - DAG 上的动态规划
 *
 * 【题目核心】
 * 1. 在有向无环图 (DAG) 中寻找从点 1 到点 n 的最长路径。
 * 2. 边权可能为负，且点编号满足 u < v。
 *
 * 【关键建模思想：天然拓扑序】
 * 1. 为什么不用跑拓扑排序？
 *    题目保证了 u < v，这意味着节点 1, 2, 3...n 本身就是一个合法的拓扑序列。
 *    我们只需要按照 1 到 n 的顺序进行遍历，就能保证在处理点 v 时，
 *    所有能到达 v 的前驱节点 u 都已经处理完毕。
 *
 * 2. 松弛操作 (Relaxation)：
 *    状态定义：dist[i] 表示从起点 1 到点 i 的当前已知最长距离。
 *    状态转移：dist[v] = max(dist[v], dist[u] + weight)
 *    这本质上是 Dijkstra 算法中松弛操作的“最大值版”。
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// --- 常量与数据结构 ---
const int MAXN = 1505;
const long long INF = 1e12; // 定义一个足够大的值作为无穷大

// 邻接表：adj[u] 存储 {终点 v, 边权 w}
// 使用 pair<int, int> 是带权图存储的标准地道写法
vector<pair<int, int>> adj[MAXN];
long long dist[MAXN];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m))
        return 0;

    // 1. 读入存图
    while (m--)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // 2. 【核心点：初始化】
    // 因为求的是最长路，且存在负权重，必须将所有点初始化为“极小值”
    // 只有起点 1 初始化为 0。这代表“目前只有点 1 是可达的”。
    for (int i = 1; i <= n; i++)
        dist[i] = -INF;
    dist[1] = 0;

    // 3. 【核心点：线性递推（基于拓扑序）】
    for (int i = 1; i <= n; i++)
    {
        // 【关键判定】：不可达判定
        // 只有起点 1 能到达的点，才能作为“源”去更新后续节点。
        // 这里用 -INF/2 是为了防止在 -INF 的基础上加了微小正权导致判等失效。
        if (dist[i] < -INF / 2)
            continue;

        for (auto &edge : adj[i])
        {
            int v = edge.first;
            int w = edge.second;

            // 执行松弛操作
            if (dist[i] + w > dist[v])
            {
                dist[v] = dist[i] + w;
            }
        }
    }

    // 4. 结果输出
    if (dist[n] < -INF / 2)
    {
        cout << -1 << endl; // 1 无法到达 n
    }
    else
    {
        cout << dist[n] << endl;
    }

    return 0;
}

/**
 * 【避坑与深度总结】 —— 整理自我的 Debug 历程
 *
 * 1. 为什么 Dijkstra 不能求最长路？
 *    Dijkstra 的贪心前提是“加入一个点后路径只会变长（或变短）”。
 *    但在最长路或含负权的图中，这个贪心策略不成立。
 *    由于本题是 DAG，拓扑序 DP 是最稳健的选择。
 *
 * 2. 初始化为何如此重要？
 *    如果我们把 dist 全部初始化为 0，程序会错误地把那些“从节点 2、3 出发”的路径
 *    也算进结果里。而题目要求的是从【节点 1】出发。
 *    设置极小值的目的：隔离不可达区域。
 *
 * 3. 变量类型：
 *    虽然本题 int 勉强够用，但处理路径累加问题时，养成用 long long 的习惯能避开很多玄学溢出。
 *
 * 4. 状态更新公式：
 *    错误：dist[v] = max(dist[v], dist[v] + w); (这是逻辑闭环)
 *    正确：dist[v] = max(dist[v], dist[u] + w); (用已知的 u 去更新未知的 v)
 */