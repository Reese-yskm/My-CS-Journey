/**
 * @file Shopping_Probability.cpp
 * @brief 条件概率与 DFS 枚举 - 商场购物问题
 *
 * 【题目描述】
 * 有 n 个人去商场，第 i 个人买东西的概率为 p_i。
 * 已知“恰好有 r 个人买了东西”（事件 E），求在这个前提下，每个人买东西的概率。
 * 即求条件概率：P(E_i | E)，其中 E_i 表示第 i 个人买东西。
 *
 * 【数学建模：贝叶斯思想】
 * 根据条件概率公式：P(E_i | E) = P(E_i ∩ E) / P(E)
 *
 * 1. 分母 P(E)：所有“恰好有 r 个人买东西”的情况的概率之和。
 * 2. 分子 P(E_i ∩ E)：在所有“恰好有 r 个人买东西”的情况中，包含“第 i 个人买了”的那些情况的概率之和。
 *
 * 我们需要通过 DFS 枚举出所有可能的“购买组合”，筛选出人数恰好为 r 的组合进行累加。
 */

#include <iostream>
#include <vector>
#include <iomanip> // 用于处理输出精度

using namespace std;

// --- 全局变量 ---
int n, r;
double p[25];              // 每个人买东西的原始概率 p_i
double individual_sum[25]; // 分子：每个人在满足条件下的概率贡献总和
double total_sum;          // 分母：所有满足条件的组合概率总和
bool buy[25];              // 记录当前 DFS 路径中，谁买了谁没买

/**
 * @brief 深度优先搜索：枚举所有人的购买状态
 * @param cur   当前决策到第几个人 (1 ~ n)
 * @param count 当前已经确定买了东西的人数
 */
void dfs(int cur, int count)
{
    // 【剪枝优化 1】：如果当前买的人数已经超过了目标 r，后面就不用看了
    if (count > r)
        return;

    // 【剪枝优化 2】：如果剩下的所有人全买，也凑不够 r 个，也直接回头
    // (n - cur + 1) 是剩下还没决策的人数
    if (count + (n - cur + 1) < r)
        return;

    // --- 递归边界：决策完所有人 ---
    if (cur == n + 1)
    {
        // 只有当刚好有 r 个人买了时，才进行结算
        if (count == r)
        {
            double current_scenario_prob = 1.0;
            // 计算当前这种“具体组合”发生的概率
            for (int i = 1; i <= n; i++)
            {
                if (buy[i])
                    current_scenario_prob *= p[i]; // 买了：乘 p_i
                else
                    current_scenario_prob *= (1.0 - p[i]); // 没买：乘 (1 - p_i)
            }

            // 1. 累加到分母：事件 E 发生的总概率
            total_sum += current_scenario_prob;

            // 2. 累加到分子：如果第 i 个人在这一组合中买了，就加到他的分子里
            for (int i = 1; i <= n; i++)
            {
                if (buy[i])
                {
                    individual_sum[i] += current_scenario_prob;
                }
            }
        }
        return;
    }

    // --- 决策分支 ---

    // 分支 1：第 cur 个人买了
    buy[cur] = true;
    dfs(cur + 1, count + 1);

    // 分支 2：第 cur 个人没买
    buy[cur] = false;
    dfs(cur + 1, count);
}

int main()
{
    // 提高输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入 n (总人数) 和 r (目标购买人数)
    // 实际 OJ 中通常有多组数据，可用 while(cin >> n >> r && (n || r))
    if (cin >> n >> r)
    {
        for (int i = 1; i <= n; i++)
        {
            cin >> p[i];
        }

        // 初始化全局变量
        total_sum = 0.0;
        for (int i = 1; i <= n; i++)
            individual_sum[i] = 0.0;

        // 从第 1 个人开始 DFS
        dfs(1, 0);

        // --- 输出结果 ---
        cout << fixed << setprecision(4); // 固定保留 4 位小数
        for (int i = 1; i <= n; i++)
        {
            // 条件概率 = 分子 / 分母
            // 注意：如果分母为 0（虽然本题保证有解），需处理异常
            if (total_sum > 0)
                cout << individual_sum[i] / total_sum << endl;
            else
                cout << "0.0000" << endl;
        }
    }

    return 0;
}

/**
 * 【关键核心知识点 & 注意事项】
 *
 * 1. DFS 的状态空间：
 *    本题的 DFS 实际上是在枚举一棵高度为 n 的二叉树，每个节点代表一个人的决策（买/不买）。
 *    总状态数是 2^n。当 n=20 时，2^20 ≈ 10^6，处于可接受范围。
 *
 * 2. 条件概率的拆解：
 *    这类题目最容易卡住的地方是不知道如何计算分子。记住：分母是所有合法组合的概率总和，
 *    分子是“合法组合”中“特定条件满足”的概率总和。
 *
 * 3. 递归边界的控制：
 *    在递归处理 cur 个人时，要注意 base case 是 cur == n + 1。
 *    如果在 cur == n 时就停止，会漏掉对最后一个人的决策处理。
 *
 * 4. 关于“抄作业”的思维提升：
 *    看思路是获取“武器图纸”，亲手打磨（Debug）是“铸造武器”。
 *    只要你能解释清楚为什么分母是所有 r 组合之和，为什么递归需要 count 变量，
 *    这个算法就已经被你真正内化了。
 */