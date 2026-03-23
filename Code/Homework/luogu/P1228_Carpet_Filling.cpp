/**
 * @file: P1228_Carpet_Filling.cpp
 * @brief: 洛谷 P1228 地毯填补问题 - 分治法经典案例
 * @author: [Your Name]
 * @date: 2024-xx-xx
 *
 * [题目背景]
 * 在 2^k * 2^k 的棋盘中，有一个特殊点（公主）。
 * 要求用 L 型地毯（覆盖3个格子的L形）覆盖除了特殊点以外的所有格子。
 *
 * [核心算法：分治 (Divide and Conquer)]
 * 1. 分解：将 2^k * 2^k 的大问题分解为 4 个 2^(k-1) * 2^(k-1) 的子问题。
 * 2. 构造：
 *    - 在 4 个象限中，只有一个象限包含“特殊点”。
 *    - 另外 3 个象限是空的，不满足“带有一个特殊点的正方形”这一递归定义。
 *    - 关键技巧：在 4 个象限交汇的中心处，放置一个 L 型地毯，覆盖那三个没有特殊点的象限。
 *    - 这样，这三个地毯覆盖的点就成为了那三个子问题的“伪特殊点”。
 * 3. 递归：现在 4 个子问题各含有一个特殊点，递归调用直到规模为 1x1。
 *
 * [时间复杂度]
 * T(n) = 4T(n/2) + O(1)
 * 根据主定理，时间复杂度为 O(4^k)，即 O(总格子数)，这是理论最优解。
 */

#include <iostream>
#include <cmath>

using namespace std;

/**
 * @param sx 子区域左上角行坐标
 * @param sy 子区域左上角列坐标
 * @param px 特殊点（或伪特殊点）行坐标
 * @param py 特殊点（或伪特殊点）列坐标
 * @param len 当前子区域边长
 */
void solve(int sx, int sy, int px, int py, int len)
{
    // 递归出口：边长为 1，无需填补
    if (len == 1)
        return;

    int h = len / 2;
    int mx = sx + h; // 中心水平分界线（下半部分起始行）
    int my = sy + h; // 中心垂直分界线（右半部分起始列）

    /**
     * 判断特殊点所在的象限并处理
     * 1. 填补中心 L 型地毯（拐角坐标及形状编号）
     * 2. 对四个象限进行递归
     * 注意：每个象限递归时，如果没有原特殊点，则传入 L 型地毯覆盖在该象限的那个格子作为“伪特殊点”
     */

    // --- 情况 1：公主在左上象限 ---
    if (px < mx && py < my)
    {
        // L型拐角在右下象限的起始点(mx, my)，开口向左上
        cout << mx << " " << my << " 1" << endl;
        // 递归四个象限
        solve(sx, sy, px, py, h);     // 左上：用原特殊点
        solve(sx, my, mx - 1, my, h); // 右上：用伪特殊点 (mx-1, my)
        solve(mx, sy, mx, my - 1, h); // 左下：用伪公主点 (mx, my-1)
        solve(mx, my, mx, my, h);     // 右下：用伪公主点 (mx, my)
    }

    // --- 情况 2：公主在右上象限 ---
    else if (px < mx && py >= my)
    {
        // L型拐角在左下象限的中心点(mx, my-1)，开口向右上
        cout << mx << " " << my - 1 << " 2" << endl;
        solve(sx, sy, mx - 1, my - 1, h); // 左上：伪特殊点
        solve(sx, my, px, py, h);         // 右上：原特殊点
        solve(mx, sy, mx, my - 1, h);     // 左下：伪特殊点
        solve(mx, my, mx, my, h);         // 右下：伪特殊点
    }

    // --- 情况 3：公主在左下象限 ---
    else if (px >= mx && py < my)
    {
        // L型拐角在右上象限的中心点(mx-1, my)，开口向左下
        cout << mx - 1 << " " << my << " 3" << endl;
        solve(sx, sy, mx - 1, my - 1, h); // 左上
        solve(sx, my, mx - 1, my, h);     // 右上
        solve(mx, sy, px, py, h);         // 左下：原特殊点
        solve(mx, my, mx, my, h);         // 右下
    }

    // --- 情况 4：公主在右下象限 ---
    else
    {
        // L型拐角在左上象限的中心点(mx-1, my-1)，开口向右下
        cout << mx - 1 << " " << my - 1 << " 4" << endl;
        solve(sx, sy, mx - 1, my - 1, h); // 左上
        solve(sx, my, mx - 1, my, h);     // 右上
        solve(mx, sy, mx, my - 1, h);     // 左下
        solve(mx, my, px, py, h);         // 右下：原特殊点
    }
}

int main()
{
    // 快速 IO 优化
    ios::sync_with_stdio(false);
    cin.tie(0);

    int k;
    if (!(cin >> k))
        return 0;

    int length = 1 << k; // 位运算计算 2^k
    int sx, sy;
    cin >> sx >> sy;

    // 开始分治求解
    solve(1, 1, sx, sy, length);

    return 0;
}

/**
 * [学习笔记总结]
 *
 * 1. 递归一致性：
 *    分治法要求子问题具有相同的形式。本题通过在地毯中心放置一个L型，
 *    巧妙地让“没有空位的子块”变成“有一个空位的子块”，从而维持递归。
 *
 * 2. 坐标计算技巧：
 *    - mx = sx + h; my = sy + h; 这两个值将区域划分为：
 *      行：[sx, mx-1] 和 [mx, sx+len-1]
 *      列：[sy, my-1] 和 [my, sy+len-1]
 *    - 中心交界处的四个点分别是：(mx-1, my-1), (mx-1, my), (mx, my-1), (mx, my)。
 *
 * 3. L型形状代码含义：
 *    - 1: 拐角在右下，覆盖(右上、左下、右下) - 修正：根据题目，1号是覆盖(右上、左下、右下)，拐角在右下。
 *    - 2: 拐角在左下，覆盖(左上、左下、右下)。
 *    - 3: 拐角在右上，覆盖(左上、右上、右下)。
 *    - 4: 拐角在左上，覆盖(左上、右上、左下)。
 */