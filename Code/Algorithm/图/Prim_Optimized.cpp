#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    vector<int> dist(n + 1, INT_MAX);
    vector<bool> visited(n + 1, false);
    while (m--)
    {
        int i, j, u;
        cin >> i >> j >> u;
        adj[i].push_back({j, u});
        adj[j].push_back({i, u});
    }
    int start;
    cin >> start;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});
    long long ans = 0;
    while (!pq.empty())
    {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u])
        {
            continue;
        }
        if (visited[u])
        {
            continue;
        }
        visited[u] = true;
        for (auto &x : adj[u])
        {
            int order = x.first;
            int weight = x.second;
            // 核心不同！
            if (weight < dist[order])
            {
                dist[order] = weight;
                pq.push({dist[order], order});
            }
        }
    }
    for (int i = 1; i <= n; i++)
    {
        ans += dist[i];
    }
    cout << ans;
    return 0;
}