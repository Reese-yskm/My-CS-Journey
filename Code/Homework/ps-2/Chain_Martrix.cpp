#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

vector<vector<int>> s; // s[i][j] 表示 A_i...A_j 的最优最后分割点 k
vector<vector<ll>> m;  // m[i][j] 表示 A_i...A_j 相乘的最少标量乘法次数

ll solve(const vector<int> &p, int n)
{
    // 初始化 DP 表
    m.assign(n + 1, vector<ll>(n + 1, 0));
    s.assign(n + 1, vector<int>(n + 1, 0));

    // 单个矩阵不需要乘法，所以 m[i][i] = 0
    for (int i = 1; i <= n; i++)
    {
        m[i][i] = 0;
    }

    // l 表示区间长度，也就是矩阵链长度
    // 从长度为 2 的区间开始算，逐步算到长度为 n
    for (int l = 2; l <= n; l++)
    {
        // i 表示区间左端点
        for (int i = 1; i <= n - l + 1; i++)
        {
            // j 表示区间右端点
            int j = i + l - 1;

            // 当前区间 A_i...A_j 的最小代价，先设为无穷大
            m[i][j] = LLONG_MAX;

            // 枚举最后一次乘法的分割点 k
            // 即把 A_i...A_j 分成：
            // A_i...A_k 和 A_{k+1}...A_j
            for (int k = i; k < j; k++)
            {
                // 左边最优代价：m[i][k]
                // 右边最优代价：m[k + 1][j]
                // 最后一次合并代价：p[i - 1] * p[k] * p[j]
                ll t = m[i][k] + m[k + 1][j] + 1LL * p[i - 1] * p[k] * p[j];

                if (t < m[i][j])
                {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }
        }
    }

    return m[1][n];
}

void print_pro(int i, int j)
{
    // 如果区间里只有一个矩阵，直接打印它
    if (i == j)
    {
        cout << "A_" << i;
        return;
    }

    // 否则，根据 s[i][j] 记录的最优分割点递归打印左右两边
    cout << "(";
    print_pro(i, s[i][j]);
    print_pro(s[i][j] + 1, j);
    cout << ")";
}

int main()
{
    int n;
    cin >> n;

    // p 数组长度为 n + 1
    // 矩阵 A_i 的规模是 p[i - 1] × p[i]
    vector<int> p(n + 1);

    for (int i = 0; i <= n; i++)
    {
        cin >> p[i];
    }

    cout << solve(p, n) << "\n";
    print_pro(1, n);

    return 0;
}