/**
 * =============================================================
 * @file    P2058_The_Harbor.cpp
 * @author  Reese_fan (NJU-CS)
 * @date    2026-03-10
 * @brief   洛谷 P2058 海港 - 滑动窗口与频率统计
 * =============================================================
 *
 * 【核心逻辑：滑动窗口 (Sliding Window)】
 *
 * 1. 挑战：我们需要统计 86400 秒这个动态窗口内的不同国籍数。
 * 2. 破局点：std::set 虽然能去重，但无法处理“过期剔除”。
 *    因为同一时刻同一国籍可能有多人，直接从 set 删掉会导致误删。
 *
 * 3. 解决方案：
 *    - 使用 std::queue 存储乘客对象（到达时间, 国籍）。
 *    - 使用 cnt[] 数组记录窗口内每个国籍出现的具体次数。
 *    - 使用 ans 记录当前窗口内次数非零的国籍总数。
 *
 * 4. 复杂度分析：
 *    - 每个乘客只进队一次，出队一次。
 *    - 总时间复杂度：O(Σki)，即所有乘客的总人数。
 * =============================================================
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 乘客模型
struct Person
{
    int time;
    int nation;
};

// 全局变量自动初始化为 0
int nationality_cnt[100005]; // 记录窗口内每个国籍的人数
int distinct_nations = 0;    // 当前窗口内不同国家的数量

int main()
{
    // 性能优化：南大计科职业习惯
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n))
        return 0;

    queue<Person> ship_queue;

    for (int i = 0; i < n; ++i)
    {
        int t, k;
        cin >> t >> k;

        // 1. 新船入队：处理当前到达的所有乘客
        while (k--)
        {
            int na;
            cin >> na;
            ship_queue.push({t, na});

            // 如果该国籍是第一次出现在窗口里，不同国家数 +1
            if (nationality_cnt[na] == 0)
            {
                distinct_nations++;
            }
            nationality_cnt[na]++;
        }

        // 2. 过期剔除：处理 24 小时之前的乘客
        int expiry_time = t - 86400;
        while (!ship_queue.empty())
        {
            Person front_person = ship_queue.front();

            // 如果队头乘客的时间在 24 小时之内，说明后续乘客都没过期
            if (front_person.time > expiry_time)
                break;

            // 否则，该乘客出队，并更新统计
            nationality_cnt[front_person.nation]--;
            // 如果该国籍在窗口内人数归零，不同国家数 -1
            if (nationality_cnt[front_person.nation] == 0)
            {
                distinct_nations--;
            }
            ship_queue.pop();
        }

        // 3. 实时输出
        cout << distinct_nations << "\n";
    }

    return 0;
}

/**
 * 【学习方法感悟】
 *
 * 关于“独立思考”的复盘：
 * 1. 算法学习并不是要“自创算法”，而是要“理解模式”并“独立实现细节”。
 * 2. 思考 20 分钟卡住后获取思路，是最高效的学习方式。真正的成长发生
 *    在将思路转化为具体 C++ 代码的过程中（如处理队列边界、优化输出效率）。
 * 3. 不要因为拿到了点拨而感到不踏实。能把点拨转化为 AC 代码，
 *    说明逻辑已经在我的键盘下完成了内化。
 * =============================================================
 */