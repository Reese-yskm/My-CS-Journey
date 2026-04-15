/**
 * @file P1525_Criminals.cpp
 * @brief 洛谷 P1525 [NOIP 2010 提高组] 关押罪犯
 *
 * 【题目核心】
 * 将 N 名罪犯分配到两个监狱，使产生的最大冲突值尽可能小。
 *
 * 【建模思路：贪心 + 扩展域并查集】
 *
 * 1. 贪心策略：
 *    - 市长只看“最大冲突值”。
 *    - 为了让最大值最小，我们应该“优先化解怨气最大的矛盾”。
 *    - 做法：将所有矛盾按怨气值 c 从大到小排序。
 *
 * 2. 扩展域并查集 (影子域)：
 *    - 开两倍空间 2*N。1~N 为本体，N+1~2N 为影子（敌对域）。
 *    - 逻辑：如果你是我敌人，你就该去我的影子所在的集合（另一个监狱）。
 *
 * 3. 冲突判定：
 *    - 按怨气值从大到小处理。
 *    - 如果某对矛盾 (u, v) 的两个人在并查集里已经【连通】了（find(u) == find(v)），
 *      由于我们是按从大到小处理的，这意味着这是我们【被迫】关在一起的第一个（也是最大的）矛盾。
 *    - 直接输出当前怨气值，退出程序。
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// N=20000，扩展域需 2*N = 40000
int fa[40005];

struct Edge
{
    int u, v, w;
};

// 贪心：按怨气值降序排列
bool cmp(const Edge &a, const Edge &b)
{
    return a.w > b.w;
}

void init(int n)
{
    for (int i = 1; i <= 2 * n; i++)
        fa[i] = i;
}

int find(int x)
{
    if (fa[x] == x)
        return x;
    return fa[x] = find(fa[x]);
}

void merge(int x, int y)
{
    int fx = find(x);
    int fy = find(y);
    if (fx != fy)
        fa[fy] = fx;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    // 1. 排序：先处理怨气大的
    sort(edges.begin(), edges.end(), cmp);

    // 2. 初始化 2*N 空间的并查集
    init(n);

    // 3. 贪心模拟
    for (auto &e : edges)
    {
        // 如果这两个人已经被迫连通（在同一个监狱）
        if (find(e.u) == find(e.v))
        {
            cout << e.w << endl;
            return 0; // 这就是能做到的最小“最大冲突”
        }

        // 否则，想方设法把他们分开：
        // 敌人的敌人是朋友
        merge(e.u, e.v + n); // u 的对头是 v
        merge(v, e.u + n);   // v 的对头是 u
    }

    // 如果所有矛盾都能化解，输出 0
    cout << 0 << endl;

    return 0;
}

/**
 * 【关键核心知识点 & 注意事项】
 *
 * 1. 贪心 + 并查集的结合：
 *    这道题的精妙之处在于排序后的处理。我们并不关心到底关在哪个监狱，
 *    只关心“两个人是否必须在一起”。
 *
 * 2. 数组越界 (RE)：
 *    初学者最容易在扩展域数组大小上犯错。N=20000 意味着下标会到 40000。
 *    记住：扩展域并查集数组大小 = 节点数 × 种类数（本题为 2 种）。
 *
 * 3. 为什么 find(u) == find(v) 就能判定冲突？
 *    因为我们一直在执行 merge(u, v+n) 和 merge(v, u+n)。
 *    如果在处理当前边之前，find(u) 已经等于 find(v) 了，
 *    说明之前的某几条更重要的边（怨气值更大的边）已经通过“敌人的敌人”逻辑
 *    强制把 u 和 v 连在了一起。
 *
 * 4. 这种思路的通用性：
 *    这种“排序 + 维护连通性”的套路在 Kruskal 算法、区间处理题目中非常多见。
 */