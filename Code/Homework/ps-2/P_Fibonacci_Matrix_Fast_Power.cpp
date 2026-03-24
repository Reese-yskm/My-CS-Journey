/**
 * @file: P_Fibonacci_Matrix_Fast_Power.cpp
 * @brief: 斐波那契数列第 n 项 (n <= 10^9) - 矩阵快速幂解法
 * @author: [Your Name]
 * @date: 2024-xx-xx
 *
 * [题目描述]
 * 定义斐波那契数列 F0 = 0, F1 = 1, Fn = Fn-1 + Fn-2 (n >= 2)。
 * 求 Fn % 998244353。
 * 数据范围：n <= 10^9。
 *
 * [深度复盘与学习笔记]
 *
 * 1. 为什么放弃线性递推？
 *    - 当 n = 10^9 时，O(n) 的循环需要约 10 秒，且 10^9 个 long long 占用约 7.45GB 内存。
 *    - 算法竞赛中，n >= 10^8 通常暗示需要 O(log n) 或 O(1) 的算法。
 *
 * 2. 矩阵递推数学模型 (Matrix Recurrence):
 *    利用矩阵乘法的结合律：
 *    [ Fn   ] = [ 1  1 ] [ Fn-1 ] = [ 1  1 ]^(n-1) [ F1 ]
 *    [ Fn-1 ]   [ 1  0 ] [ Fn-2 ]   [ 1  0 ]       [ F0 ]
 *
 *    令基础矩阵 M = [ 1  1 ]
 *                  [ 1  0 ]
 *    计算 M^(n-1) 后，结果矩阵的左上角元素即为 Fn。
 *
 * 3. 分治思想的应用 (Binary Exponentiation):
 *    计算 M^n 采用快速幂（分治）：
 *    - Divide: n 分解为 n/2。
 *    - Conquer: 递归计算 M^(n/2)。
 *    - Combine: M^n = M^(n/2) * M^(n/2) (奇数则多乘一次 M)。
 *    复杂度：O(2^3 * log n)，其中 2^3 是矩阵乘法常数。
 *
 * 4. 溢出防范 (Overflow Prevention):
 *    - 核心：(a * b) % MOD。
 *    - 中间变量：(10^9 * 10^9) = 10^18，在 long long (9*10^18) 范围内，
 *      故每次乘法后必须立即取模，防止加法溢出。
 */

#include <iostream>

using namespace std;

typedef long long ll;
const ll MOD = 998244353;

// 定义 2x2 矩阵结构体
struct Matrix
{
    ll w, x, y, z; // 对应矩阵：[ w  x ]
                   //           [ y  z ]
};

/**
 * 矩阵乘法函数
 * 实现：C = A * B，带即时取模防止溢出
 */
Matrix multiply(Matrix a, Matrix b)
{
    return {
        (a.w * b.w % MOD + a.x * b.y % MOD) % MOD,
        (a.w * b.x % MOD + a.x * b.z % MOD) % MOD,
        (a.y * b.w % MOD + a.z * b.y % MOD) % MOD,
        (a.y * b.x % MOD + a.z * b.z % MOD) % MOD};
}

/**
 * 矩阵快速幂 (分治实现)
 * @param base 基础转移矩阵
 * @param n 幂次
 * @return base^n
 */
Matrix matrix_qpow(Matrix base, ll n)
{
    // 单位矩阵 I (相当于数值计算中的 1)
    Matrix res = {1, 0, 0, 1};
    while (n > 0)
    {
        if (n & 1)
        { // 如果 n 是奇数
            res = multiply(res, base);
        }
        base = multiply(base, base); // 分治：A^n = A^(n/2) * A^(n/2)
        n >>= 1;                     // n /= 2
    }
    return res;
}

int main()
{
    // 优化标准流输入输出
    ios::sync_with_stdio(false);
    cin.tie(0);

    ll n;
    if (!(cin >> n))
        return 0;

    // 特殊情况处理
    if (n == 0)
    {
        cout << 0 << endl;
        return 0;
    }
    if (n == 1)
    {
        cout << 1 << endl;
        return 0;
    }

    // 基础转移矩阵 [ 1 1 ]
    //              [ 1 0 ]
    Matrix base = {1, 1, 1, 0};

    /**
     * 根据推导：M^(n-1) 的结果矩阵为
     * [ Fn   Fn-1 ]
     * [ Fn-1 Fn-2 ]
     * 因此我们需要的结果是矩阵的左上角元素 w
     */
    Matrix ans = matrix_qpow(base, n - 1);

    cout << ans.w % MOD << endl;

    return 0;
}

/**
 * [收获总结]
 * 1. 维度切换：从线性思维 (O(n)) 切换到对数思维 (O(log n))。
 * 2. 构造模型：将代数递推式转化为几何/矩阵变换。
 * 3. 稳健编程：在大规模模运算中，long long 和即时取模是 AC 的基石。
 */