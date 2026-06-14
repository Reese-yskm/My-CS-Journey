#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
// 返回长度不大于k的最大子段和
ll maxSubarrayAtMostK(const vector<ll> &a, int k)
{
    int n = a.size();
    vector<ll> pre(n + 1, 0);
    for (int i = 1; i <= n; i++)
    {
        pre[i] = pre[i - 1] + a[i - 1];
    }
    //[l,r]的区间和等于pre[r+1]-pre[l]
    deque<int> q;
    q.push_back(0);
    ll ans = LLONG_MIN;
    // 枚举右端点后一位
    for (int i = 1; i <= n; i++)
    {
        while (!q.empty() && q.front() < i - k)
        {
            q.pop_front();
        }
        // 更新最大值
        if (!q.empty())
        {
            ans = max(ans, pre[i] - pre[q.front()]);
        }
        while (!q.empty() && pre[q.back()] >= pre[i])
        {
            q.pop_back();
        }
        q.push_back(i);
    }
    return ans;
}
ll maxSubarrayAtLeastK(const vector<ll> &a, int k)
{
    int n = a.size();
    vector<ll> pre(n + 1, 0);
    for (int i = 1; i <= n; i++)
    {
        pre[i] = pre[i - 1] + a[i - 1];
    }
    //[l,r]的区间和等于pre[r+1]-pre[l]
    ll ans = LLONG_MIN;
    ll minPre = LLONG_MAX;
    for (int i = k; i <= n; i++)
    {

        minPre = min(minPre, pre[i - k]);
        ans = max(ans, pre[i] - minPre);
    }

    return ans;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    ll x;
    cin >> n >> k >> x;
    vector<ll> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    // 如果区间长度小于等于k,等价于a[i]+x的区间最大子段和
    vector<ll> b(n);
    for (int i = 0; i < n; i++)
    {
        b[i] = a[i] + x;
    }
    ll temp1 = maxSubarrayAtMostK(b, k);
    // 如果区间长度大于k,等价于a[i]-x,最后加上2kx
    vector<ll> c(n);
    for (int i = 0; i < n; i++)
    {
        c[i] = a[i] - x;
    }
    ll ans = temp1;

    if (k + 1 <= n)
    {
        ll temp2 = maxSubarrayAtLeastK(c, k + 1);
        ans = max(ans, temp2 + 2LL * k * x);
    }
    cout << max(0LL, ans);
    return 0;
}
