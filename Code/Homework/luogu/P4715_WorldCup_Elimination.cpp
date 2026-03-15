/**
 * ======================================================================================
 * @file    P4715_WorldCup_Elimination.cpp
 * @author  Reese_fan (NJU-CS)
 * @date    2026-03-15
 * @brief   洛谷 P4715 淘汰赛 - 分治法模拟二叉树赛程
 * ======================================================================================
 *
 * 【分治心法：寻找各级优胜者】
 *
 * 1. 核心观察：
 *    淘汰赛是一个天然的二叉树结构。每个节点的胜者由左右子节点的比赛产生。
 *    最终的冠军产生于：左半区的最强者 vs 右半区的最强者。
 *
 * 2. 递归定义：
 *    solve(l, r) 返回区间 [l, r] 内能力值最强的国家“编号”。
 *    - Base Case: 如果 l == r，只有一个国家，胜者就是它自己。
 *    - 分治：分别求左、右半区的胜者，然后通过比较能力值，返回真正的强者。
 *
 * 3. 亚军判定：
 *    亚军 = min_ability(solve(左半区), solve(右半区))。
 * ======================================================================================
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Country
{
    int id;
    int ability;
};

// 全局或传引用，存储所有国家信息
vector<Country> countries;

// 分治函数：返回区间 [l, r] 中最强国家的编号
int get_winner_id(int l, int r)
{
    // 基础情况：区间只有一个国家，它就是该区间的优胜者
    if (l == r)
    {
        return countries[l].id;
    }

    int mid = l + (r - l) / 2;
    int left_best_id = get_winner_id(l, mid);
    int right_best_id = get_winner_id(mid + 1, r);

    // 比较两个半区的最强者，返回更强者的 ID
    // 注意：countries 数组下标和 ID 需要对应好。如果 ID 是 1-based 且直接做下标：
    if (countries[left_best_id].ability > countries[right_best_id].ability)
    {
        return left_best_id;
    }
    else
    {
        return right_best_id;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n))
        return 0;

    int total = 1 << n; // 2^n 的位运算写法，更 Pro
    countries.resize(total + 1);

    for (int i = 1; i <= total; i++)
    {
        countries[i].id = i;
        cin >> countries[i].ability;
    }

    // 重点：决赛发生在左半区冠军和右半区冠军之间
    int mid = total / 2;
    int left_champ = get_winner_id(1, mid);
    int right_champ = get_winner_id(mid + 1, total);

    // 决赛中输掉的那个人（能力值小的）是亚军
    if (countries[left_champ].ability > countries[right_champ].ability)
    {
        cout << countries[right_champ].id << endl;
    }
    else
    {
        cout << countries[left_champ].id << endl;
    }

    return 0;
}

/**
 * 【学长复盘】
 *
 * 1. 关于递归边界：
 *    分治时尽量使用最小原子边界（size=1）。你之前的 p+1 == r 假设了区间
 *    总是偶数且总是能完美对半分，这在处理不规则区间时非常容易死循环。
 *
 * 2. 空间换时间：
 *    虽然 n 很小（2^n <= 128），但这种分治结构其实就是“线段树”的简化版。
 *    理解了这种“左半区最强 vs 右半区最强”的思维，你就理解了单调区间维护的本质。
 *
 * 3. 编程细节：
 *    (1 << n) 代替 pow(2, n) 可以避免浮点数精度带来的麻烦，是计科人的常规操作。
 */