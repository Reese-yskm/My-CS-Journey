/**
 * @file: Matrix_Multiplication_Verification_Freivalds.cpp
 * @brief: 矩阵乘法验证 - Freivalds 算法 (随机化校验)
 * @author: [Your Name]
 * @date: 2024-xx-xx
 *
 * [题目背景]
 * 给定三个 n*n 矩阵 A, B, C，判断 AB = C 是否成立。
 * 数据范围：n = 1000。
 *
 * [核心痛点]
 * 直接计算 A * B 的复杂度是 O(n^3)，即 1000^3 = 10^9。
 * 在 1s 限制下，10^9 次运算几乎一定会超时 (TLE)。
 *
 * [算法原理：Freivalds' Algorithm]
 * 1. 数学基础：利用矩阵乘法的 [结合律]。
 *    如果 AB = C，那么对于任何向量 x，(AB)x = Cx 一定成立。
 *    根据结合律：A(Bx) = Cx。
 *
 * 2. 降维打击：
 *    - 计算 Bx：矩阵 (n*n) 乘以向量 (n*1)，复杂度 O(n^2)。得到向量 y。
 *    - 计算 Ay：矩阵 (n*n) 乘以向量 (n*1)，复杂度 O(n^2)。得到向量 z。
 *    - 计算 Cx：矩阵 (n*n) 乘以向量 (n*1)，复杂度 O(n^2)。得到向量 w。
 *    - 比较 z 和 w：复杂度 O(n)。
 *    总复杂度从 O(n^3) 降到了 O(n^2)。
 *
 * 3. 概率保证：
 *    如果 AB != C，随机选取一个向量 x，使得 A(Bx) = Cx 成立的概率小于 1/2。
 *    如果我们测试 k 轮（使用不同的随机向量 x），错误概率将降至 (1/2)^k。
 *    当 k = 10 时，错误概率约为 0.00097；当 k = 20 时，几乎为 0。
 *
 * [避坑指南]
 * 1. 变量重名 (Shadowing)：全局变量和局部变量同名会导致局部覆盖，切记不要在 main 中二次定义 n。
 * 2. 随机种子：mt19937 引擎应放在循环外或设为 static，否则 time(0) 1秒内的种子相同，多次随机失效。
 * 3. 数值溢出：n=1000, 元素=500, 随机数=10^6，中间结果可达 10^17，必须使用 long long。
 */

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

typedef long long ll;

int n; // 全局变量，避免局部重名遮蔽

/**
 * 矩阵乘以向量的 O(n^2) 实现
 */
vector<ll> mat_mul_vec(const vector<vector<ll>> &mat, const vector<ll> &x)
{
    vector<ll> res(n, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[i] += mat[i][j] * x[j];
        }
    }
    return res;
}

// 随机数生成引擎，放在外部确保种子状态连续
mt19937_64 rng(time(0));

/**
 * Freivalds 单轮校验
 */
bool check(const vector<vector<ll>> &A, const vector<vector<ll>> &B, const vector<vector<ll>> &C)
{
    // 生成随机向量 x
    uniform_int_distribution<ll> dist(0, 1000000);
    vector<ll> x(n);
    for (int i = 0; i < n; i++)
        x[i] = dist(rng);

    // 计算 L = A(Bx)
    vector<ll> bx = mat_mul_vec(B, x);
    vector<ll> abx = mat_mul_vec(A, bx);

    // 计算 R = Cx
    vector<ll> cx = mat_mul_vec(C, x);

    // 逐元素比对向量
    for (int i = 0; i < n; i++)
    {
        if (abx[i] != cx[i])
            return false;
    }
    return true;
}

int main()
{
    // 快速 IO
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n))
        return 0;

    // 读入矩阵 A, B, C
    vector<vector<ll>> A(n, vector<ll>(n));
    vector<vector<ll>> B(n, vector<ll>(n));
    vector<vector<ll>> C(n, vector<ll>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i][j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> B[i][j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> C[i][j];

    // 多轮校验提高准确率
    bool is_equal = true;
    int k = 10; // 测试 10 轮
    while (k--)
    {
        if (!check(A, B, C))
        {
            is_equal = false;
            break;
        }
    }

    if (is_equal)
        cout << "Yes" << endl;
    else
        cout << "No" << endl;

    return 0;
}

/**
 * [学习笔记总结]
 *
 * 1. 算法思维的转变：
 *    不再追求 100% 的确定性，而是利用 [随机性] 在极短时间内获得 [高概率正确结果]。
 *    这种思想在现代密码学、分布式计算（如 PBFT 算法）中非常重要。
 *
 * 2. 结合律的威力：
 *    矩阵乘法不满足交换律，但严格满足 [结合律]。
 *    (A*B)*x = A*(B*x) 是将计算量从立方降到平方的数学支点。
 *
 * 3. 随机化不仅是技巧，更是一种应对“恶心数据”的防御。
 *    出题人很难构造出一个能让所有随机向量都失效的 AB 和 C 组合。
 */