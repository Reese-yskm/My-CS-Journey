/**
 * @file P1983_Station_Grading.cpp
 * @brief 洛谷 P1983 [NOIP 2013 普及组] 车站分级 - 隐式建图与拓扑排序最长路
 *
 * 【题目核心】
 * 规则：一趟火车停靠了若干站。在始发站到终点站之间，凡是它“没停”的站，级别一定低于“停了”的站。
 * 目标：求出所有车站至少需要划分为多少个级别。
 *
 * 【建模思考：隐式建图】
 * 1. 逻辑转化：
 *    “级别低” -> “级别高” 可以看作一条有向边：(未停靠站) -> (已停靠站)。
 *    如果有 A -> B，意味着 Level(A) < Level(B)，即 Level(B) 至少是 Level(A) + 1。
 *
 * 2. 核心难点：边数爆炸与去重
 *    假设一趟车经过 1000 站，停了 500 站，未停 500 站。
 *    这一趟车就会产生 500 * 500 = 250,000 条逻辑边。
 *    M=1000 趟车如果不去重，边数会达到千万级，导致 TLE 或内存溢出。
 *    方案：使用 bool matrix `has_edge[1005][1005]` 记录，只有从未建立过的边才加入邻接表。
 *
 * 3. 求解：
 *    这本质上是求 DAG（有向无环图）上的【最长路径节点数】。
 *    使用拓扑排序：f[v] = max(f[v], f[u] + 1)。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// --- 全局变量 ---
const int MAXN = 1005;
vector<int> adj[MAXN];     // 邻接表
int in_degree[MAXN];       // 入度
bool has_edge[MAXN][MAXN]; // 边去重矩阵（由于 N=1000，10^6 的 bool 数组仅占 1MB）
int f[MAXN];               // 记录每个站点的最高级别

int main()
{
    // 1. 优化 I/O 效率
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m))
        return 0;

    // 2. 读入车次并建图
    for (int k = 0; k < m; k++)
    {
        int num;
        cin >> num;
        vector<int> stops(num);
        vector<bool> is_stop(n + 1, false);

        for (int i = 0; i < num; i++)
        {
            cin >> stops[i];
            is_stop[stops[i]] = true; // 标记哪些站在这趟车中停了
        }

        // 区间限定：始发站到终点站
        int start_node = stops[0];
        int end_node = stops.back();

        // 遍历区间内的每一个站
        for (int i = start_node; i <= end_node; i++)
        {
            if (!is_stop[i])
            { // 如果这个站没停，说明它级别低
                for (int target : stops)
                { // 对于每一个停了的站（级别高）
                    if (!has_edge[i][target])
                    { // 关键：如果还没建立过这条逻辑边
                        adj[i].push_back(target);
                        has_edge[i][target] = true; // 标记已建立，防止重复计入入度
                        in_degree[target]++;
                    }
                }
            }
        }
    }

    // 3. 拓扑排序计算最长路
    queue<int> q;
    for (int i = 1; i <= n; i++)
    {
        if (in_degree[i] == 0)
        {
            f[i] = 1; // 没有任何人比它小，级别起步为 1
            q.push(i);
        }
    }

    int total_levels = 0;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        total_levels = max(total_levels, f[u]);

        for (int v : adj[u])
        {
            // 状态转移：v 的级别至少比指向它的 u 高一级
            f[v] = max(f[v], f[u] + 1);

            if (--in_degree[v] == 0)
            {
                q.push(v);
            }
        }
    }

    // 4. 输出结果
    cout << total_levels << endl;

    return 0;
}

/**
 * 【避坑指南 & 深度复盘】 —— 整理自我的 Debug 历程
 *
 * 1. 变量复用陷阱：
 *    - 错误尝试：while(n--) 读入数据。
 *    - 真相：n 是总车站数，一旦在读入第一趟车时减到 0，后面的逻辑全部报废。
 *    - 启示：循环变量和全局规模变量必须分清。
 *
 * 2. 内存与效率的平衡：
 *    - 在图论中，重复边是性能的“杀手”。
 *    - 题目给出的 M 和 N 虽然都是 1000，但隐含的边数规模可能极大。
 *    - bool 矩阵去重是解决此类“区间逻辑建图”题目的标准手段。
 *
 * 3. 算法的物理意义：
 *    - “至少分多少级” = “DAG 路径上的最大节点数”。
 *    - 在这道题里，拓扑排序的每一“层”其实就对应了一个车站等级。
 *
 * 4. 深刻领悟：
 *    - 建模的第一步是寻找“偏序关系”（即谁大谁小）。
 *    - 只要确定了偏序关系，就能连边。
 *    - 连完边后，问题就从“车站分级”降级为了“拓扑排序”。
 */