#include <iostream>
#include <vector>
#include <queue>

using namespace std;

bool visitied_dfs[1005];
bool visitied_bfs[1005];

void dfs(vector<vector<int>> &adj, int n)
{
    visitied_dfs[n] = true;
    cout << n << " ";
    for (auto x : adj[n])
    {
        if (!visitied_dfs[x])
        {
            dfs(adj, x);
        }
    }
}

void bfs(vector<vector<int>> &adj, int start)
{
    queue<int> q;
    q.push(start);
    visitied_bfs[start] = true;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        cout << u << " ";
        for (auto v : adj[u])
        {
            if (!visitied_bfs[v])
            {
                visitied_bfs[v] = true;
                q.push(v);
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    // 方案 A：开 n+1 大小，下标直接对齐节点编号 1~n
    vector<vector<int>> graph(n + 1, vector<int>(n + 1, 0));
    vector<vector<int>> adj(n + 1); // 推荐用 vector 套 vector 代替 数组套 vector

    while (m--)
    {
        int i, j;
        cin >> i >> j;
        // 注意是双向
        graph[i][j] = 1;
        graph[j][i] = 1;
        adj[i].push_back(j);
        adj[j].push_back(i);
    }
    dfs(adj, 2);
    cout << endl;
    bfs(adj, 2);

    return 0;
}