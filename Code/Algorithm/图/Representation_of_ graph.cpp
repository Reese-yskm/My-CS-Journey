#include <iostream>
#include <vector>

using namespace std;

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
        graph[i][j] = 1;
        graph[j][i] = 1;
        adj[i].push_back(j);
        adj[j].push_back(i);
    }

    int aim;
    cin >> aim;

    // 矩阵查询
    cout << "Neighbors of " << aim << " (Matrix): ";
    for (int i = 1; i <= n; i++) // 从 1 遍历到 n
    {
        if (graph[aim][i])
        {
            cout << i << " ";
        }
    }
    cout << endl;

    // 邻接表查询
    cout << "Neighbors of " << aim << " (Vector): ";
    for (auto x : adj[aim])
    {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}