/**
 * @file: P2404_Natural_Number_Partition.cpp
 * @brief: 自然数的拆分问题 - DFS 与 回溯
 * @author: [Your Name]
 * @date: 2024-xx-xx
 *
 * [题目要求]
 * 1. 拆分：将 n 拆分为若干个小于 n 的自然数之和。
 * 2. 顺序：拆分序列内部必须从小到大（非递减）。
 * 3. 字典序：优先输出字典序小的序列。
 *
 * [核心逻辑：DFS 三部曲]
 * 1. 状态定义：dfs(remaining_sum, start_num)
 *    - remaining_sum: 当前还剩多少目标值需要凑齐。
 *    - start_num: 为了保证非递减顺序，当前选取的数字必须大于等于上一个选的数。
 *
 * 2. 剪枝与去重：
 *    - 为了避免搜到重复组合（如 1+2 和 2+1），我们强制规定序列是非递减的。
 *    - 循环从 start_num 开始，这样搜出来的结果天然就是有序且唯一的。
 *
 * 3. 回溯（Backtracking）：
 *    - “踏入”：将当前数字加入 path，并减去目标值。
 *    - “探险”：递归调用 dfs。
 *    - “撤回”：将当前数字从 path 中移除，尝试下一个可能的数字。
 */

#include <iostream>
#include <vector>

using namespace std;

int n;
vector<int> path; // 记录当前的拆分路径

/**
 * @param to_sum 剩余需要凑齐的总和
 * @param cur    当前搜索的起始数字（保证非递减）
 */
void dfs(int to_sum, int cur)
{
    // --- 边界条件：凑够了 n ---
    if (to_sum == 0)
    {
        // 题目要求拆分成“若干个”数，且每个数 < n
        // path.size() <= 1 说明只选了 n 本身，不符合题意
        if (path.size() > 1)
        {
            for (int i = 0; i < path.size(); i++)
            {
                cout << path[i] << (i == path.size() - 1 ? "" : "+");
            }
            cout << endl;
        }
        return;
    }

    // --- 尝试每一个可能的数字 i ---
    for (int i = cur; i <= to_sum; i++)
    {
        // 约束：拆分出的数字必须小于 n
        if (i == n)
            continue;

        path.push_back(i);  // 【踏】：记录选择
        dfs(to_sum - i, i); // 【探】：递归，下一位数字至少要等于 i
        path.pop_back();    // 【撤】：回溯，恢复状态
    }
}

int main()
{
    // 快速 IO
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n))
        return 0;

    // 从总和 n 开始，最小拆分数字从 1 开始
    dfs(n, 1);

    return 0;
}

/**
 * [深度复盘]
 *
 * 1. 为什么 i 从 cur 开始？
 *    这是典型的 [组合去重] 技巧。如果我们每次都从 1 开始尝试，
 *    会搜出 {1,2} 和 {2,1}。通过限制 i >= cur，我们只保留了 {1,2}，
 *    既保证了题目要求的“从小到大排序”，又避免了重复计算。
 *
 * 2. 为什么 i <= to_sum？
 *    这是搜索的 [截止条件]。最后剩下的那一块 to_sum 必须能被
 *    填入 path，所以循环必须包含等于 to_sum 的情况。
 *
 * 3. 字典序的天然属性：
 *    由于 DFS 优先尝试小的数字（i 从小到大循环），
 *    因此最先找到的解一定是字典序最小的（如 1+1+...），无需额外排序。
 *
 * 4. 关于 n 的限制：
 *    本题 n 很小 (<=8)，所以 DFS 可以轻松跑完。
 *    如果是 n 很大（如 100），这就会变成一个动态规划（DP）中的 [整数拆分] 问题。
 */