/**
 * @file P1219_Checker_Challenge.cpp
 * @brief 洛谷 P1219 [USACO1.5] 八皇后 Checker Challenge - 回溯算法
 *
 * 【题目描述】
 * 在 N*N 的棋盘上放置 N 个皇后，使得每行、每列、每条对角线上至多有一个棋子。
 * 输出前三个解（字典序）及解的总数。
 *
 * 【建模思考：全排列 + 剪枝】
 * 1. 行约束：通过 DFS 的深度控制，每一层递归处理一行，天然保证每行只有一个。
 * 2. 列约束：使用一个布尔数组 `col_used` 记录哪一列放了皇后。
 * 3. 对角线约束：这是本题的数学核心。
 *    - 主对角线 (\ 方向)：行号与列号之差为常数。即 r - c = const。
 *      为了防止负数下标，使用 r - c + n 作为索引。
 *    - 副对角线 (/ 方向)：行号与列号之和为常数。即 r + c = const。
 */

#include <iostream>
#include <vector>

using namespace std;

// --- 全局变量 ---
int n;
int total_ans = 0;  // 总解数
int path[15];       // 记录每一行皇后放在了哪一列
bool col_used[30];  // 列标记
bool main_diag[30]; // 主对角线标记 (\方向, r-c+n)
bool sub_diag[30];  // 副对角线标记 (/方向, r+c)

/**
 * @brief 深度优先搜索（回溯）
 * @param row 当前正在处理第几行
 */
void dfs(int row)
{
    // 递归出口：成功放完了 n 行
    if (row > n)
    {
        total_ans++;
        // 题目要求：只打印前三个解
        if (total_ans <= 3)
        {
            for (int i = 1; i <= n; i++)
            {
                cout << path[i] << (i == n ? "" : " ");
            }
            cout << "\n";
        }
        return;
    }

    // 尝试在当前行的每一列放置皇后
    for (int col = 1; col <= n; col++)
    {
        // --- 核心判断：列、主对角线、副对角线是否都未被占领 ---
        if (!col_used[col] && !main_diag[row - col + n] && !sub_diag[row + col])
        {

            // 1. 尝试占领位置（标记状态）
            path[row] = col;
            col_used[col] = true;
            main_diag[row - col + n] = true;
            sub_diag[row + col] = true;

            // 2. 递归：处理下一行
            dfs(row + 1);

            // 3. 回溯（重置状态）
            // 这是最重要的一步：退出当前递归层时，必须把标记抹去，
            // 这样才能让下一轮循环（尝试其他列）或其他分支正常运行。
            col_used[col] = false;
            main_diag[row - col + n] = false;
            sub_diag[row + col] = false;
        }
    }
}

int main()
{
    // 优化输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n))
        return 0;

    // 从第 1 行开始搜索
    dfs(1);

    // 输出总解数
    cout << total_ans << endl;

    return 0;
}

/**
 * 【重点总结与深度思考】
 *
 * 1. 字典序的保证：
 *    - 我们在每一行都是从 col = 1 遍历到 n。
 *    - DFS 会优先探索列号更小的分支，因此找到的解天然符合字典序。
 *
 * 2. 对角线数组大小为什么是 30？
 *    - r + c 的最大值是 n + n = 26 (当 n=13 时)。
 *    - r - c + n 的最大值是 13 - 1 + 13 = 25。
 *    - 开 30 是为了留出足够的安全边界。
 *
 * 3. 为什么需要回溯？
 *    - DFS 本质是在一棵巨大的“搜索树”上移动。
 *    - “标记”相当于在当前路径上插旗子。当我们从一个死胡同退回到分叉口，
 *      必须把之前插的旗子拔掉，否则会影响我们探索其他正确路径。
 *
 * 4. 剪枝思想：
 *    - 这个程序没有生成所有的全排列再检查，而是在放每一行时就提前检查冲突。
 *    - 这种“一旦发现不合法就立即停止并返回”的操作叫“可行性剪枝”，极大地提高了效率。
 */