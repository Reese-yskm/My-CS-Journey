#include <iostream>
#include <vector>
#include <queue>      // 包含 priority_queue (优先队列)
#include <functional> // 包含 greater<int> 用于构建最小堆

using namespace std;

/**
 * 【算法导论：第16章 贪心算法 - 合并果子笔记】
 *
 * 1. 贪心策略：
 *    每次从当前的果子堆中，选出质量最小的两堆进行合并。
 *
 * 2. 为什么这是最优的？（赫夫曼编码思想）：
 *    合并的总消耗 = 所有初始果子堆重量 * 它被合并的次数。
 *    为了让总消耗最小，我们要让重量大的果子堆参与合并的次数尽可能少（留在后面合并），
 *    让重量小的果子堆参与合并的次数尽可能多（先进行合并）。
 *
 * 3. 为什么需要优先队列（最小堆）？
 *    - 如果每合并一次就用 sort 排序，复杂度为 O(n^2 log n)，会超时。
 *    - 堆（Heap）可以在 O(log n) 时间内插入新元素，并在 O(log n) 内取出最小值。
 *    - 总复杂度为 O(n log n)，完美通过 10^4 的数据量。
 */

int main()
{
    int n;
    // 输入果子种类数
    if (!(cin >> n))
        return 0;

    // 定义一个最小堆 (Min-Heap)
    // priority_queue 默认是大顶堆，通过 greater<int> 参数可以将其转为小顶堆
    priority_queue<long long, vector<long long>, greater<long long>> pq;

    for (int i = 0; i < n; i++)
    {
        long long temp;
        cin >> temp;
        pq.push(temp); // 将所有果子压入堆中，复杂度 O(n log n)
    }

    long long total_cost = 0; // 用于记录总消耗的体力值

    // 只要堆里还有超过一堆果子，就继续合并
    while (pq.size() > 1)
    {
        // 第一步：贪心选择当前最小的两堆
        long long first = pq.top();
        pq.pop();

        long long second = pq.top();
        pq.pop();

        // 第二步：计算本次合并的消耗
        long long current_combined = first + second;
        total_cost += current_combined;

        // 第三步：将合并后的新果子堆重新放回堆中
        // 这一步是关键！新堆会根据重量自动在堆中找到它正确的位置（O(log n)）
        pq.push(current_combined);
    }

    // 最后堆中只剩一堆果子，输出累加的总体力消耗
    cout << total_cost << endl;

    return 0;
}

/**
 * 【学习建议】
 * 1. 数据类型：虽然题目保证结果小于 2^31，但习惯上处理这种累加问题建议用 long long。
 * 2. 优先队列：std::priority_queue 的底层就是你在第6章学过的二叉堆。
 * 3. 思考：如果这道题要求合并必须是“相邻”的两堆，贪心算法还会生效吗？
 *    （答案：不会，那时候就需要用到第15章的动态规划 - 区间DP了）。
 */