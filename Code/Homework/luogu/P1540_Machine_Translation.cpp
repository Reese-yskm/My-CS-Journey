/**
 * ======================================================================================
 * @file    P1540_Machine_Translation.cpp
 * @author  Reese_fan (NJU-CS)
 * @date    2026-03-01
 * @brief   洛谷 P1540 机器翻译 - FIFO 缓存置换模拟
 * ======================================================================================
 *
 * 【题目背景】NOIP 2010 提高组 T1
 * 【核心逻辑】
 * 模拟一个容量为 M 的内存缓存：
 * 1. 单词不在内存：查词典（res++），若内存满则弹出最早进入的单词，将新单词入队。
 * 2. 单词在内存：直接翻译，不产生开销。
 *
 * ======================================================================================
 * 【算法建模：FIFO 队列 + 布尔哈希】
 *
 * 1. 容器选择：
 *    - std::queue: 用于维护内存中单词的进入顺序，实现“先进先出”的删除逻辑。
 *    - std::vector<bool>: 作为“频率数组”或“存在哈希”，实现 O(1) 级别的查找。
 *
 * 2. 时间复杂度：O(N)，其中 N 是文章长度。每个单词入队出队各一次。
 * 3. 空间复杂度：O(M + MAX_WORD_VALUE)，内存占用极低。
 * ======================================================================================
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
    // 性能优化
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int m, n;
    if (!(cin >> m >> n))
        return 0;

    int lookup_count = 0;                // 查词典次数
    queue<int> memory_q;                 // 模拟内存块（FIFO）
    vector<bool> in_memory(1005, false); // 标记单词是否在内存中

    for (int i = 0; i < n; i++)
    {
        int word;
        cin >> word;

        // 如果单词不在内存中（Cache Miss）
        if (!in_memory[word])
        {
            lookup_count++; // 必须去外存查词典

            // 如果内存已满，根据 FIFO 原则腾出空间
            if (memory_q.size() >= m)
            {
                int oldest_word = memory_q.front();
                memory_q.pop();
                in_memory[oldest_word] = false; // 同步更新标记位
            }

            // 将新单词放入内存
            memory_q.push(word);
            in_memory[word] = true;
        }
        // 如果在内存中（Cache Hit），不做任何操作
    }

    cout << lookup_count << endl;

    return 0;
}

/**
 * 【反思与感悟】
 *
 * 1. 算法直觉：这道题是约瑟夫问题（环形队列）的变体，重点在于理解“先进先出”的物理意义。
 * 2. 优化意识：初学者容易在每次查询时遍历队列，而使用 bool 数组进行状态标记，
 *    是将 $O(N \cdot M)$ 优化到 $O(N)$ 的关键。
 * 3. 系统联系：这本质上是操作系统中的 FIFO 页面置换算法。通过这道题，我理解了
 *    Cache 命中与失效的基本处理流程，这对于以后理解计算机体系结构非常有帮助。
 */