/**
 * ======================================================================================
 * @file    P1908_Inversion_Count.cpp
 * @author  Reese_fan (NJU-CS)
 * @brief   逆序对计数 - 归并排序实现 (O(n log n))
 * ======================================================================================
 *
 * 【学长复盘：70分到100分的质变】
 *
 * 1. 中间变量溢出：
 *    即使最终结果是 long long，函数内部累加逆序对的 temp 变量也必须是 long long。
 *    在 N=5e5 级别的数据下，单次合并产生的逆序对就能爆掉 int。
 *
 * 2. 避开哨兵：
 *    虽然《算法导论》中为了教学方便使用了 INT_MAX 哨兵，但在竞赛中，数据范围
 *    经常会触及 int 的极限。改用 while(i < n1 && j < n2) 是工业级稳健写法。
 * ======================================================================================
 */

#include <iostream>
#include <vector>

using namespace std;

// 合并函数，返回该区间内的逆序对数量
long long merge(vector<int> &a, int p, int q, int r)
{
    long long count = 0; // 必须是 long long
    int n1 = q - p + 1;
    int n2 = r - q;

    // 拷贝数据到临时数组
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = a[p + i];
    for (int j = 0; j < n2; j++)
        R[j] = a[q + 1 + j];

    int i = 0, j = 0, k = p;

    // 标准归并逻辑
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            a[k++] = L[i++];
        }
        else
        {
            // 当左边比右边大时，说明 L[i] 及其后面所有的 L 元素都比 R[j] 大
            // 这些都是相对于 R[j] 的逆序对
            a[k++] = R[j++];
            count += (n1 - i);
        }
    }

    // 扫尾逻辑
    while (i < n1)
        a[k++] = L[i++];
    while (j < n2)
        a[k++] = R[j++];

    return count;
}

long long find_inversion(vector<int> &a, int p, int r)
{
    if (p >= r)
        return 0;
    int q = p + (r - p) / 2;

    long long res = 0;
    res += find_inversion(a, p, q);
    res += find_inversion(a, q + 1, r);
    res += merge(a, p, q, r);

    return res;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    cout << find_inversion(a, 0, n - 1) << endl;

    return 0;
}