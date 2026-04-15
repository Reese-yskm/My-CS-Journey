/**
 * @file P1892_GangGroups.cpp
 * @brief 洛谷 P1892 [团伙] - 扩展域并查集 (种类并查集)
 *
 * 【题目描述】
 * 1. 朋友的朋友是朋友 -> 标准并查集合并。
 * 2. 敌人的敌人是朋友 -> 核心难点：并查集只能处理“同类”关系，如何处理“对立”关系？
 *
 * 【建模核心：扩展域（影子域）思想】
 * 我们给每个人开一个“影子”，代表他的“敌对阵营”。
 * 数组大小开到 2*n：
 * - 1 ~ n: 朋友域（本体）。
 * - n+1 ~ 2n: 敌人域（影子）。
 *
 * 逻辑转换：
 * - F p q：本体合并。merge(p, q)。
 * - E p q：敌人的敌人是朋友。
 *   即：让 q 加入 p 的“敌人域”，同时让 p 加入 q 的“敌人域”。
 *   merge(p + n, q);
 *   merge(q + n, p);
 * 这样，如果再来一个 E q k，k 也会加入 q 的敌人域（也就是 p 所在的域），
 * 于是 p 和 k 就在并查集里自然连通了！
 */

#include <iostream>
#include <vector>
#include <set>

using namespace std;

// 2n 最大为 2000，数组稍微开大一点
int fa[2010];

void init(int n)
{
    for (int i = 1; i <= 2 * n; i++)
        fa[i] = i;
}

int find(int x)
{
    if (fa[x] == x)
        return x;
    return fa[x] = find(fa[x]); // 路径压缩
}

void merge(int x, int y)
{
    int fx = find(x);
    int fy = find(y);
    if (fx != fy)
        fa[fy] = fx;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    init(n);

    while (m--)
    {
        char opt;
        int p, q;
        cin >> opt >> p >> q;
        if (opt == 'F')
        {
            merge(p, q);
        }
        else
        {
            // 【核心建模】：敌人的敌人是朋友
            merge(p + n, q);
            merge(q + n, p);
        }
    }

    /**
     * 【重点讲解：最后的统计坑点】
     *
     * 错误做法：for(i=1~n) if(fa[i] == i) res++;
     * 原因：由于我们引入了影子域，一个真实人物 i 的“根节点”可能会指向影子域（编号 > n）。
     *      如果只判断 i <= n 的根，会漏掉这些“迁徙”到影子域的集合。
     *
     * 正确做法：统计 1 ~ n 这 n 个真实的人，一共有多少个不同的【最终根节点】。
     * 使用 find(i) 配合 set 去重是最稳健的。
     */
    set<int> groups;
    for (int i = 1; i <= n; i++)
    {
        groups.insert(find(i)); // 找到每个人最终的大 Boss
    }

    cout << groups.size() << endl;

    return 0;
}

/**
 * 【关键核心知识点 & 注意事项】
 *
 * 1. 扩展域的威力：
 *    并查集不仅仅能维护“属于同一类”，通过开辟额外的空间（影子域），
 *    它能维护“对立”、“环状食物链”等复杂逻辑。
 *
 * 2. 统计逻辑：
 *    在扩展域并查集中，统计结果时必须调用 find(i) 来获取真正的根。
 *    不要直接访问 fa[i]，也不要只统计特定范围内的根。
 *
 * 3. 数组大小：
 *    既然是扩展域，数组空间和初始化循环必须是 2 倍（甚至 3 倍，视题目逻辑而定）。
 *
 * 4. 这种思路的跨越：
 *    你通过这道题学会了“通过增加维度来简化逻辑”。
 *    这种“影子变量”的思想在后续处理更复杂的逻辑冲突（如 P1525 关押罪犯）时是绝对的神兵利器。
 */