/**
 * @file P1135_Strange_Elevator.cpp
 * @brief 洛谷 P1135 奇怪的电梯 - 最短路径问题 (BFS 建模)
 *
 * 【题目描述】
 * 大楼有 N 层，每层有一个数字 Ki。在第 i 层，你只能选择向上跳 Ki 层或向下跳 Ki 层。
 * 求从 A 层到 B 层最少需要按几次键。如果无法到达，输出 -1。
 *
 * 【核心建模思考】
 * 1. 为什么是 BFS（广度优先搜索）？
 *    - 题目要求的是“最少次数”。
 *    - 每一跳的“代价”都是相同的（按 1 次键）。
 *    - 在这种权值相等的图中，BFS 第一次到达目标的路径一定是最短路径。
 *
 * 2. 为什么 DFS 在这题容易出问题？
 *    - 容易陷入死循环（比如 1层跳到3层，3层又跳回1层）。
 *    - 需要额外的逻辑来不断更新最小值，效率不如 BFS。
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 定义电梯状态：当前在哪一层，已经按了多少次键
struct Status
{
    int layer;
    int step;
};

int main()
{
    // 1. 输入处理
    int n, a, b;
    if (!(cin >> n >> a >> b))
        return 0;

    vector<int> k(n + 1);
    for (int i = 1; i <= n; i++)
    {
        cin >> k[i];
    }

    // --- 核心建模：BFS 准备工作 ---
    queue<Status> q;
    vector<bool> visited(n + 1, false); // 关键：防止死循环的“防重墙”

    // 起点入队
    q.push({a, 0});
    visited[a] = true;

    int result = -1; // 默认结果为 -1

    // --- BFS 主过程 ---
    while (!q.empty())
    {
        // A. 获取队首信息（必须在 pop 之前提取完整，或存入局部变量）
        Status cur = q.front();
        q.pop();

        // B. 目标检测
        if (cur.layer == b)
        {
            result = cur.step;
            break; // 第一次找到的就是最短的，直接退出
        }

        // C. 尝试两个分支：向上跳和向下跳
        int up = cur.layer + k[cur.layer];
        int down = cur.layer - k[cur.layer];

        // 向上跳逻辑
        if (up <= n && !visited[up])
        {
            visited[up] = true;         // 标记已访问，防止后面的人再把它加入队列
            q.push({up, cur.step + 1}); // 步数在当前基础上 +1
        }

        // 向下跳逻辑
        if (down >= 1 && !visited[down])
        {
            visited[down] = true;
            q.push({down, cur.step + 1});
        }
    }

    // 3. 输出最终结果
    cout << result << endl;

    return 0;
}

/**
 * 【避坑指南与深度总结】
 *
 * 1. 难点：如何识别 BFS 建模？
 *    - 只要是求“最少步数”、“最短距离”，且每步权重相等，首选 BFS。
 *    - 它可以把复杂的楼层跳转想象成一张图，BFS 就像水波扩散，先碰到目标的波纹一定路径最短。
 *
 * 2. 坑点一：Visited 数组的重要性
 *    - 在这道题中，如果没有 visited 标记，1->3, 3->1 这样的循环会导致队列爆炸（Memory Limit Exceeded）。
 *    - 记住：在 BFS 中，只要一个节点被“看到”并准备入队，它就已经拿到了到达它的最短步数，后续再看到它都可以忽略。
 *
 * 3. 坑点二：代码实现的细节 (q.pop 的陷阱)
 *    - 错误做法：q.pop() 后直接使用 q.front()。如果此时队列空了，程序会 Crash。
 *    - 正确做法：先用一个变量/结构体把 q.front() 存下来，再 pop。
 *
 * 4. 坑点三：复制粘贴手误
 *    - 写完“向上跳”的逻辑去写“向下跳”时，务必检查所有的 `+` 是否改成了 `-`，所有的边界条件是否改成了 `>=1`。
 *
 * 总结：BFS = 队列 + 状态结构体 + 访问标记 + 边界检查。
 */