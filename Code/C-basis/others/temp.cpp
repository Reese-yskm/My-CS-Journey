#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

typedef long long ll;
const int MOD = 998244353;

struct Matrix
{
    ll v[20][20];
    int size;
    Matrix(int s) : size(s)
    {
        memset(v, 0, sizeof(v));
    }

    Matrix operator*(const Matrix &b) const
    {
        Matrix res(size);
        for (int i = 0; i < size; i++)
        {
            for (int k = 0; k < size; k++)
            {
                if (v[i][k] == 0)
                    continue;
                for (int j = 0; j < size; j++)
                {
                    res.v[i][j] = (res.v[i][j] + v[i][k] * b.v[k][j]) % MOD;
                }
            }
        }
        return res;
    }
};

Matrix qpow(Matrix a, ll b)
{
    Matrix res(a.size);
    for (int i = 0; i < a.size; i++)
        res.v[i][i] = 1;
    while (b > 0)
    {
        if (b & 1)
            res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

ll power(ll a, ll b)
{
    ll res = 1;
    a %= MOD;
    while (b > 0)
    {
        if (b & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

ll C[10][10];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, k;
    ll c;
    if (!(cin >> m >> k >> c))
        return 0;

    for (int i = 0; i <= 10; i++)
    {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++)
            C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
    }

    vector<ll> a(m + 1), f(m + 1), b(k + 1);
    for (int i = 1; i <= m; i++)
        cin >> a[i];
    for (int i = 1; i <= m; i++)
        cin >> f[i];
    for (int i = 0; i <= k; i++)
        cin >> b[i];

    // 状态向量 V_n = [f_n, f_n-1, ..., f_n-m+1, c^n, n^k, n^k-1, ..., n^0]
    int size = m + k + 2;
    Matrix M(size);

    for (int j = 1; j <= m; j++)
        M.v[0][j - 1] = a[j] % MOD;
    M.v[0][m] = c % MOD;
    for (int p = 0; p <= k; p++)
    {
        ll coeff = 0;
        for (int i = p; i <= k; i++)
        {
            coeff = (coeff + b[i] % MOD * C[i][p]) % MOD;
        }
        M.v[0][m + 1 + (k - p)] = coeff;
    }

    for (int i = 1; i < m; i++)
        M.v[i][i - 1] = 1;

    M.v[m][m] = c % MOD;

    for (int p = 0; p <= k; p++)
    {
        int row = m + 1 + (k - p);
        for (int j = 0; j <= p; j++)
        {
            int col = m + 1 + (k - j);
            M.v[row][col] = C[p][j];
        }
    }

    int q;
    cin >> q;
    while (q--)
    {
        ll qi;
        cin >> qi;
        if (qi <= m)
        {
            cout << f[qi] % MOD << "\n";
            continue;
        }

        vector<ll> Vm(size);
        for (int i = 0; i < m; i++)
            Vm[i] = f[m - i] % MOD;
        Vm[m] = power(c, m);
        for (int i = 0; i <= k; i++)
            Vm[m + 1 + (k - i)] = power(m, i);

        Matrix resM = qpow(M, qi - m);

        ll ans = 0;
        for (int i = 0; i < size; i++)
        {
            ans = (ans + resM.v[0][i] * Vm[i]) % MOD;
        }
        cout << ans << " ";
    }

    return 0;
}