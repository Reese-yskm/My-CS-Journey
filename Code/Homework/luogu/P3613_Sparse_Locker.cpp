/**
 * @file P3613_Sparse_Locker.cpp
 * @author Reese_fan (NJU-CS)
 * @brief 洛谷 P3613 寄包柜 - 使用 map 处理稀疏二维数据
 * @date 2026-03-01
 *
 * [心法总结]
 * 1. 逻辑空间 vs 物理空间：逻辑上我们可以拥有无限大的坐标系，但物理上只需为“存在”的数据付钱。
 * 2. 容器选择：std::map 底层是红黑树，虽然比数组慢，但在处理极其稀疏的巨型矩阵时是救命稻草。
 * 3. 慎用 resize：在处理 vector<vector> 时，resize 会强制申请连续内存，极易导致 MLE。
 */

#include <iostream>
#include <map>
#include <vector>

using namespace std;

// 定义全局 map 数组，存储柜子信息
// locker[i] 对应第 i 个柜子，其内部 map 存储 {格子号: 物品ID}
const int MAXN = 100005;
map<int, int> locker[MAXN]; // 本质是连续的数组，每一个元素都是一个map，学会这种写法及其用法

int main()
{
    // 快速输入输出优化
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q))
        return 0;

    while (q--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int i, j, k;
            cin >> i >> j >> k;
            if (k == 0)
            {
                // 如果物品为 0，代表清空，erase 可以彻底释放该节点的内存
                locker[i].erase(j);
            }
            else
            {
                // 动态插入或修改
                locker[i][j] = k;
            }
        }
        else
        {
            int i, j;
            cin >> i >> j;
            // map 的 [] 操作符：若 key 不存在则返回默认值 0，符合题目要求
            // 更一般的情况是需要提前判断，要不然会反复生成0占用内存
            cout << locker[i][j] << "\n";
        }
    }

    return 0;
}