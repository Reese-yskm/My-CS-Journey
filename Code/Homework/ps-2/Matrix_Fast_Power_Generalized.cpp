/**
 * @file: Matrix_Fast_Power_Generalized.cpp
 * @brief: 通用线性递推数列项求解 - 矩阵快速幂终极版
 * @author: [Your Name]
 * @date: 2024-xx-xx
 *
 * [题目描述]
 * 给定递推式：fn = a1*fn-1 + a2*fn-2 + ... + am*fn-m + (b0 + b1*n + ... + bk*n^k) + c^n
 * 给定 f1...fm，求第 n 项对 998244353 取模的结果。
 * 数据范围：m <= 10, k <= 3, n <= 10^18, q <= 1000。
 *
 * [核心难点：非齐次项处理]
 * 1. 指数项 c^n：
 *    利用 c^n = c * c^(n-1)，我们可以将 c^n 放入状态向量，转移系数为常数 c。
 *
 * 2. 多项式项 n^k：
 *    由于 n 是变化的，矩阵必须是常数的。利用二项式定理：
 *    n^p = ((n-1) + 1)^p = sum_{j=0 to p} ( C(p, j) * (n-1)^j )
 *    这意味着 n 的 p 次幂可以由 (n-1) 的 0 到 p 次幂线性组合而成。
 *
 * [状态向量构造]
 * 定义 Vn = [fn, fn-1, ..., fn-m+1, c^n, n^k, n^(k-1), ..., n^0]^T
 * 矩阵大小 Size = m + 1 + (k + 1)
 *
 * [转移矩阵 M 的功能区划分]
 * 1. 左上角 (m*m)：标准齐次递推块（a1...am 和单位阵平移）。
 * 2. 中间行 (第 m+1 行)：指数项更新，M[m][m] = c。
 * 3. 右下角 ((k+1)*(k+1))：杨辉三角块，利用组合数实现 (n-1)^p -> n^p。
 * 4. 第一行 (Row 0)：将所有项汇总计算 fn。
 *    注意：fn 依赖的是 n^i，需要展开为 (n-1) 的形式：
 *    fn = ... + sum_{i=0 to k} ( bi * sum_{p=0 to i} ( C(i,p) * (n-1)^p ) )
 * 核心要点：
 * 1.深刻掌握递推式和矩阵递推的关系，理解这一类优化；
 * 2.作为语法基础去学习cpp中的矩阵类，因为你给的答案里的那个结构体是有复用性的，尤其其中还有标准的矩阵乘法的剪枝优化这个要掌握；
 * 3.学会构造组合数使用递推的方式，因为之前我做了oj会写提前阶乘打表，取模用逆元的写法，这中基础的线性递推也得会
 */

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

typedef long long ll;
const int MOD = 998244353;

struct Matrix
{
    ll v[25][25];
    int size;
    Matrix(int s) : size(s)
    {
        memset(v, 0, sizeof(v));
    }
    // 矩阵乘法，带 0 剪枝优化
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

// 矩阵快速幂
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

// 数值快速幂
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

ll C[15][15]; // 组合数

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, k;
    ll c_val;
    if (!(cin >> m >> k >> c_val))
        return 0;

    // 1. 预处理组合数 (杨辉三角)
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

    int size = m + 1 + (k + 1);
    Matrix M(size);

    // 2. 构造转移矩阵 M 的系数
    // --- 第一行：计算 fn ---
    // 处理 f 部分
    for (int j = 0; j < m; j++)
        M.v[0][j] = a[j + 1] % MOD;
    // 处理指数项贡献 c^n = c * c^(n-1)
    M.v[0][m] = c_val % MOD;
    // 处理多项式项贡献 sum(bi * n^i) -> 展开为 (n-1)^p 的组合
    for (int p = 0; p <= k; p++)
    {
        ll coeff = 0;
        for (int i = p; i <= k; i++)
        {
            coeff = (coeff + b[i] % MOD * C[i][p]) % MOD;
        }
        M.v[0][m + 1 + (k - p)] = coeff; // k-p 对应向量中 (n-1)^p 的位置
    }

    // --- 平移行：fn-1, fn-2... ---
    for (int i = 1; i < m; i++)
        M.v[i][i - 1] = 1;

    // --- 指数更新行：c^n = c * c^n-1 ---
    M.v[m][m] = c_val % MOD;

    // --- 多项式进化行：杨辉三角块 ---
    for (int p = 0; p <= k; p++)
    {
        int row = m + 1 + (k - p);
        for (int j = 0; j <= p; j++)
        {
            int col = m + 1 + (k - j);
            M.v[row][col] = C[p][j];
        }
    }

    int q_queries;
    cin >> q_queries;
    while (q_queries--)
    {
        ll q_n;
        cin >> q_n;
        if (q_n <= m)
        {
            cout << f[q_n] % MOD << "\n";
            continue;
        }

        // 3. 构造初始向量 Vm
        vector<ll> Vm(size);
        for (int i = 0; i < m; i++)
            Vm[i] = f[m - i] % MOD;
        Vm[m] = power(c_val, m);
        for (int i = 0; i <= k; i++)
            Vm[m + 1 + (k - i)] = power(m, i);

        // 4. 计算 M^(q_n - m)
        Matrix resM = qpow(M, q_n - m);

        // 5. 计算结果：resM 的第一行点乘 Vm
        ll final_ans = 0;
        for (int i = 0; i < size; i++)
        {
            final_ans = (final_ans + resM.v[0][i] * Vm[i]) % MOD;
        }
        cout << final_ans << "\n";
    }

    return 0;
}

/**
 * [复盘感悟]
 * 1. 升维思考：当递推式中有“变量”n时，通过二项式展开将变化量转化为状态向量内部的线性组合。
 * 2. 矩阵分块：将大矩阵逻辑划分为 [递推区]、[指数区]、[多项式进化区]，降低构造难度。
 * 3. 初始状态：矩阵幂计算的是“跨度”，初始向量必须与矩阵转移的逻辑时刻对齐（本题对齐到 n=m）。
 * 4. AI工具论：将折磨人的矩阵构造逻辑外包给AI/数学推导，自己负责逻辑拼装与代码落地。
 */