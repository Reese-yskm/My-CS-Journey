/**
 * ======================================================================================
 * @file    P1160_Queue_Arrangement.cpp
 * @author  Reese_fan (NJU-CS)
 * @date    2026-03-09
 * @brief   洛谷 P1160 队列安排 - 静态数组实现双向链表
 * ======================================================================================
 *
 * 【学长复盘：链表操作的艺术】
 *
 * 1. 顺序是灵魂：
 *    修改下标时，必须先让新节点(i)指向它的前驱(k)和后继(next)，再修改前驱和后继的指向。
 *    否则，一旦前驱的指向被提前修改，原本的后继节点就成了“断线的风筝”，再也找不回来了。
 *
 * 2. 哨兵节点 (Sentinel)：
 *    使用 0 号元素作为“虚拟头节点”。无论队列怎么变，0 号的右边永远是第一个人。
 *    这消除了“插入到头节点左边”这种特殊情况的判断，代码极其整洁。
 *
 * 3. 空间换时间：
 *    利用 L[] 和 R[] 数组，我们实现了 O(1) 定位、O(1) 插入、O(1) 删除。
 *    这是处理 10^5 规模动态序列的最优解。
 * ======================================================================================
 */

#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 100005;
int L[MAXN], R[MAXN];
bool removed[MAXN]; // 记录同学是否被移出

// 辅助函数：将节点 i 插入到节点 k 的右边
void add_right(int k, int i)
{
    int next_node = R[k];
    // 1. i 认主
    L[i] = k;
    R[i] = next_node;
    // 2. 两端改口
    R[k] = i;
    L[next_node] = i;
}

// 辅助函数：将节点 i 插入到节点 k 的左边
void add_left(int k, int i)
{
    int prev_node = L[k];
    // 1. i 认主
    R[i] = k;
    L[i] = prev_node;
    // 2. 两端改口
    L[k] = i;
    R[prev_node] = i;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // 初始化：0 号作为哨兵，其右边是 1 号
    R[0] = 1;
    L[1] = 0;
    // 注意：链表的末尾也应该指向哨兵 0，形成一个逻辑闭环（或将末尾 R 设为 0）
    R[1] = 0;

    for (int i = 2; i <= n; i++)
    {
        int k, p;
        cin >> k >> p;
        if (p == 0)
            add_left(k, i);
        else
            add_right(k, i);
    }

    int m;
    cin >> m;
    while (m--)
    {
        int x;
        cin >> x;
        // 只有没被删除的才执行删除逻辑
        if (!removed[x])
        {
            // 让 x 的邻居们互相“握手”，跳过 x
            R[L[x]] = R[x];
            L[R[x]] = L[x];
            removed[x] = true; // 标记已删除
        }
    }

    // 遍历输出：从哨兵 0 号的右边开始
    int cur = R[0];
    bool first = true;
    while (cur != 0)
    {
        if (!first)
            cout << " ";
        cout << cur;
        cur = R[cur];
        first = false;
    }
    cout << endl;

    return 0;
}