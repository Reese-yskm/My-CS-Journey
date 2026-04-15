/**
 * @file P5250_Wood_Warehouse.cpp
 * @brief 洛谷 P5250 【深基17.例5】木材仓库 - std::set 的深度应用
 *
 * 【题目核心】
 * 1. 动态维护一个不重复的有序集合。
 * 2. 查找“最接近”的元素：这是二分查找（Binary Search）的变体。
 * 3. 元素删除：查找到之后需要将其移出集合。
 */

#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;
    set<int> s;

    while (m--)
    {
        int op, l;
        cin >> op >> l;

        if (op == 1)
        { // 进货
            if (s.count(l))
                cout << "Already Exist\n";
            else
                s.insert(l);
        }
        else
        { // 出货
            if (s.empty())
            {
                cout << "Empty\n";
                continue;
            }

            // --- 核心逻辑：寻找最接近的木材 ---

            // 如果正好有这个长度，直接取走
            if (s.count(l))
            {
                cout << l << "\n";
                s.erase(l);
            }
            else
            {
                // 利用 set 内部的二分查找寻找第一个 > l 的位置
                auto it = s.lower_bound(l);

                // 【重点讲解：分类讨论边界情况】
                if (it == s.begin())
                {
                    // 情况1：仓库里所有木材都比要求的长
                    cout << *it << "\n";
                    s.erase(it);
                }
                else if (it == s.end())
                {
                    // 情况2：仓库里所有木材都比要求的短
                    // 注意：这里的 it 指向的是末尾后的空白位置，必须向前移动一步
                    auto it_last = prev(it);
                    cout << *it_last << "\n";
                    s.erase(it_last);
                }
                else
                {
                    // 情况3：l 夹在两个木材中间，比较前驱(p)和后继(q)
                    auto it_p = prev(it); // 比 l 小的最大的那个
                    int p = *it_p;
                    int q = *it; // 比 l 大的最小的那个

                    if (l - p <= q - l)
                    { // 距离相等取短的，所以用 <=
                        cout << p << "\n";
                        s.erase(it_p);
                    }
                    else
                    {
                        cout << q << "\n";
                        s.erase(it);
                    }
                }
            }
        }
    }
    return 0;
}

/**
 * 【学习总结：我犯的两个核心错误】
 *
 * 1. 性能陷阱：全局 lower_bound vs 成员 lower_bound
 *    - 错误写法：std::lower_bound(s.begin(), s.end(), l);
 *    - 原因：set 的迭代器是“双向迭代器”而非“随机访问迭代器”。
 *      全局函数在这个容器上会退化为 O(n) 的暴力查找。
 *    - 正确写法：s.lower_bound(l);
 *      这利用了红黑树的特性，保证 O(log n) 时间复杂度。
 *
 * 2. 指针陷阱：s.end() 是不可访问的“悬崖”
 *    - 错误写法：cout << *s.end();
 *    - 真相：s.end() 指向的是【最后一个元素之后】的位置。解引用它会导致
 *      Runtime Error (崩溃)。
 *    - 修正：如果查找结果返回 end()，说明仓库所有东西都比你小，
 *      你需要取走的是最后一个元素，即 prev(s.end())。
 *
 * 3. 逻辑细节：tie-breaker (平局处理)
 *    - 题目要求“如果两根木头距离一样，取短的那根”。
 *    - 这意味着在比较 `l-p` 和 `q-l` 时，必须使用 `<= q-l`，确保优先选出小的 p。
 *
 * 4. 关于 erase 的性能：
 *    - s.erase(p) 按值删除，内部会再查一次树。
 *    - s.erase(it) 直接删除迭代器位置，速度更快。
 */