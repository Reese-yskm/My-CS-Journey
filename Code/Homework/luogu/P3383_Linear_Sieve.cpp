/**
 * @file P3383_Linear_Sieve.cpp
 * @brief 洛谷 P3383 【模板】线性筛素数 (又称欧拉筛 Euler Sieve)
 *
 * 【题目描述】
 * 在 O(N) 的时间复杂度内找出 1 到 N 范围内的所有质数，并处理多次查询。
 *
 * 【线性筛的核心原理：最小质因子原则】
 * 埃氏筛的缺陷：一个合数会被多个质因子重复筛掉（例如 12 被 2 筛一次，被 3 又筛一次）。
 * 欧拉筛的目标：让每个合数【只被它最小的质因子】筛掉一次。
 *
 * 【算法复杂度】
 * 时间复杂度：O(N)，每个数字仅被访问常数次。
 * 空间复杂度：O(N)，需要一个标记数组和一个存放质数的数组。
 */

#include <iostream>
#include <vector>

using namespace std;

// --- 全局变量（10^8 数据量，建议开在全局防止栈溢出） ---
const int MAXN = 100000005; // 10^8
bool is_not_prime[MAXN];    // 标记是否不是质数。false 表示是质数
int primes[6000000];        // 10^8 以内大约有 576 万个质数
int cnt = 0;                // 记录当前找到的质数个数

/**
 * @brief 线性筛核心函数
 * @param n 筛选范围
 */
void sieve(int n)
{
    // 1 和 0 不是质数
    is_not_prime[0] = is_not_prime[1] = true;

    for (int i = 2; i <= n; i++)
    {
        // 如果当前数没被标记，它就是质数
        if (!is_not_prime[i])
        {
            primes[++cnt] = i;
        }

        // --- 核心：用当前数 i 去乘以所有已知的质数 ---
        for (int j = 1; j <= cnt; j++)
        {
            // 1. 边界检查：乘积超出范围则停止
            if (i * primes[j] > n)
                break;

            // 2. 标记合数：
            // 这是关键！primes[j] 此时一定是 (i * primes[j]) 的最小质因子。
            is_not_prime[i * primes[j]] = true;

            /**
             * 【重点讲解：i % primes[j] == 0 为什么要 break？】
             *
             * 这就是线性筛的“灵魂”。
             * 当 i % primes[j] == 0 时，说明 i 本身已经包含了 primes[j] 这个质因子。
             * 如果我们继续往下乘 primes[j+1]，那么 (i * primes[j+1]) 的最小质因子
             * 依然是 primes[j]，而不是 primes[j+1]。
             *
             * 逻辑推演：
             * 假设 i = 4, primes[j] = 2.
             * 我们筛掉 4 * 2 = 8.
             * 此时 4 % 2 == 0, 触发 break.
             * 如果不 break，代码会尝试去筛 4 * 3 = 12.
             * 但 12 的最小质因子是 2，它应该由 (6 * 2) 来筛，而不是由 (4 * 3) 来筛。
             *
             * 结论：只有及时 break，才能保证每个合数只被它的最小质因子筛掉。
             */
            if (i % primes[j] == 0)
                break;

            /**
             * 【关于顺序的血泪教训】
             * 必须先标记 is_not_prime[i * primes[j]]，再判断 i % primes[j] == 0。
             * 否则当 i=4, p=2 时，你会先 break 掉，导致 8 这个合数被漏掉！
             */
        }
    }
}

int main()
{
    // 针对 10^8 级别的数据，必须开启 I/O 优化
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q))
        return 0;

    // 预处理：跑一遍线性筛
    sieve(n);

    // 处理查询
    while (q--)
    {
        int k;
        cin >> k;
        // 输出第 k 个质数
        cout << primes[k] << "\n";
    }

    return 0;
}

/**
 * 【学习总结与进阶建议】
 *
 * 1. 空间意识：
 *    10^8 的 bool 数组占用约 100MB。在内存极其苛刻的题目中（如 64MB），
 *    可以使用 std::bitset<MAXN> 来将内存占用压缩到 12.5MB。
 *
 * 2. 筛选顺序：
 *    - 遍历 i (2 -> N)
 *    - 标记合数 (i * primes[j])
 *    - 判断整除并 break
 *    这一套流程不能乱，它是 O(N) 复杂度的数学基石。
 *
 * 3. 应用场景：
 *    线性筛不仅仅能找质数，通过稍微修改代码，它还能在 $O(N)$ 时间内计算出：
 *    - 每一个数的最小质因子。
 *    - 欧拉函数 (φ)。
 *    - 莫比乌斯函数 (μ)。
 *    - 约数个数/约数之和。
 */