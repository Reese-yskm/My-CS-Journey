/**
 * @file P1364_Hospital_Setting.cpp
 * @brief 洛谷 P1364 医院设置 - 树转图 + 全源最短路 (BFS 实现)
 *
 * 【题目描述】
 * 在一棵有 n 个结点的二叉树上建立一个医院。
 * 每个结点 i 有居民人口 w[i]。医院建在某个结点后，所有居民到医院的距离之和最小。
 * 距离定义：相邻结点间距离为 1（包括父子关系）。
 *
 * 【建模难点与核心讨论】
 *
 * 1. 树转图 (Tree to Graph)：
 *    - 题目给出的是二叉树（父找子），但在算距离时，子也可以找父。
 *    - 我们需要记录：左儿子 l[i]、右儿子 r[i]、以及父节点 p[i]。
 *    - 这样，每个点都有三个潜在的“邻居”，二叉树就变成了一个无向图。
 *
 * 2. 全源最短路 (All-Pairs Shortest Path)：
 *    - 既然 n 只有 100，我们可以对每一个结点 i 都跑一次 BFS。
 *    - 这次 BFS 的目的是算出：i 到所有其他结点 j 的最短距离 dist[i][j]。
 *
 * 3. 统计结果：
 *    - 枚举医院建立在结点 k。
 *    - 总路程 sum = Σ (w[j] * dist[k][j])，其中 j 从 1 到 n。
 *    - 找到最小的 sum。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// --- 数据结构 ---
const int MAXN = 105;
const int INF = 1e9;

int n;
int w[MAXN];          // 人口
int l[MAXN];          // 左儿子
int r[MAXN];          // 右儿子
int p[MAXN];          // 父亲
int dist[MAXN][MAXN]; // dist[i][j] 表示 i 到 j 的最短距离

struct State
{
    int id;
    int d;
};

/**
 * @brief 跑一遍 BFS，算出从 start 出发到所有点的最短距离
 */
void bfs(int start)
{
    // 每次 BFS 都需要独立的 visited 数组，防止在图里“鬼打墙”
    bool visited[MAXN] = {false};
    queue<State> q;

    q.push({start, 0});
    visited[start] = true;

    while (!q.empty())
    {
        State cur = q.front();
        q.pop();

        int u = cur.id;
        int d = cur.d;
        dist[start][u] = d; // 记录起点到当前点的距离

        // 尝试三个方向扩散：左、右、上（父）
        int neighbors[3] = {l[u], r[u], p[u]};
        for (int v : neighbors)
        {
            // 如果邻居存在且没去过
            if (v != 0 && !visited[v])
            {
                visited[v] = true;
                q.push({v, d + 1});
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n))
        return 0;

    // 1. 读入并构建“无向图”
    for (int i = 1; i <= n; i++)
    {
        int u, v;
        cin >> w[i] >> u >> v;
        l[i] = u;
        r[i] = v;
        if (u != 0)
            p[u] = i; // 记录父亲
        if (v != 0)
            p[v] = i; // 记录父亲
    }

    // 2. 核心：跑 n 次 BFS 填满距离矩阵
    for (int i = 1; i <= n; i++)
    {
        bfs(i);
    }

    // 3. 枚举医院位置，计算最小加权和
    long long min_total_dist = 2e18; // 使用 long long 防止溢出，初始化为极大值

    for (int k = 1; k <= n; k++)
    { // 假设医院建在第 k 个点
        long long current_sum = 0;
        for (int j = 1; j <= n; j++)
        {
            current_sum += (long long)w[j] * dist[k][j];
        }
        if (current_sum < min_total_dist)
        {
            min_total_dist = current_sum;
        }
    }

    cout << min_total_dist << endl;

    return 0;
}

/**
 * 【避坑与自省指南】 —— 整理自我们讨论的 Bug
 *
 * 1. 索引映射误区：
 *    - 读入时直接用循环变量 i 作为当前结点的编号。
 *    - 错误做法是把 w[i] 当作索引。记住：编号是唯一的，人口是属性。
 *
 * 2. BFS 的 Visited：
 *    - 在“树转图”后，图中存在回路（父<->子）。
 *    - 必须加 visited 数组，否则 BFS 会在父子结点间反复跳跃导致死循环。
 *
 * 3. 距离记录的对象：
 *    - 在 bfs(start) 内部，我们要更新的是 dist[start][u] = d。
 *    - 也就是“全局起始点”到“当前探测点”的距离。
 *
 * 4. 循环边界：
 *    - 统计总路程时，记得遍历 1 到 n 所有的点，不要漏掉最后一个。
 *
 * 5. 建模总结：
 *    - 虽然题目说是二叉树，但在求距离的问题上，树就是特殊的图。
 *    - 只要补全了父节点指针，你就能在树上“反复横跳”，轻松解决距离问题。
 */