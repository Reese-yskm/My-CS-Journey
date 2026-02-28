#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

int parent[100005];

// 初始化：每个人一开始都是自己的祖师爷
void init(int n)
{
    for (int i = 1; i <= n; i++)
        parent[i] = i;
}

// 寻找祖师爷（带路径压缩：直接让后辈记住祖师爷，下次找得飞快）
int find(int i)
{
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent[i]); // 路径压缩：这一行是 $O(\alpha(n))$ 效率的关键
}

// 合并两个门派
void unite(int i, int j)
{
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j)
        parent[root_i] = root_j;
}

struct edge
{
    int u;
    int v;
    int w;
};

bool cmp(const edge &a, const edge &b)
{
    return a.w < b.w;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    init(n);
    vector<edge> adj(m);
    for (int i = 0; i < m; i++)
    {
        cin >> adj[i].u >> adj[i].v >> adj[i].w;
    }
    sort(adj.begin(), adj.end(), cmp);
    long long res = 0;
    // 该算法核心在于找到最小权值的边之后，需要判断其两端端点是否已经连通，这里采用并查集
    // 如果两个端点有共同的祖先，则它们连通，继续遍历；
    // 否则连接他们记录权值，然后把他们各自的祖先找出来，使其成为各自的祖先，这样相当于使得他们连通
    for (int i = 0; i < m; i++)
    {
        if (find(adj[i].u) != find(adj[i].v))
        {
            res += adj[i].w;
            unite(adj[i].u, adj[i].v);
        }
    }
    cout << res;
    return 0;
}