/**
 * @file P3405_Cities_and_States.cpp
 * @brief 洛谷 P3405 - 特征指纹与动态计数配对
 *
 * 【题目核心】
 * 1. 寻找“特殊对”：城市A的前两位 == 城市B的州代码，且 城市B的前两位 == 城市A的州代码。
 * 2. 限制条件：两个城市必须来自不同的州。
 *
 * 【建模思考：从具体到抽象】
 * 1. 具体城市名不重要：我们只需要“城市名前两位(prefix)”和“州代码(state)”。
 * 2. 特征化：将每个城市抽象为一个指纹字符串 "prefix + state"。
 *    - 例如：MIAMI FL -> "MIFL"
 *    - 它的完美配对目标是 "FLMI"。
 */

#include <iostream>
#include <string>
#include <map>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // --- 核心工具：Map + 累加计数 ---
    // key: 特征指纹 (prefix + state)
    // value: 该特征指纹出现的次数
    map<string, long long> cnt;
    long long res = 0; // 结果可能很大，用 long long

    while (n--)
    {
        string city, state;
        cin >> city >> state;

        string prefix = city.substr(0, 2);

        // 【关键逻辑 1：过滤同州】
        // 题目要求必须来自不同州。如果 prefix == state（如 MIAMI MI），直接跳过。
        if (prefix != state)
        {
            string current_fingerprint = prefix + state;
            string target_fingerprint = state + prefix;

            // 【关键逻辑 2：为什么要“累加”而不是“res++”？】
            // 假设之前已经出现了 3 个特征为 "FIMI" 的城市（比如 FLINT, FLICK, FLOK...）。
            // 此时新进来一个城市 "MIAMI FL"，它的目标是 "FIMI"。
            // 这个新城市可以分别和那 3 个城市各组成一对。
            // 所以，res 应该增加的是“之前已经存在的配对目标数量”。
            if (cnt.count(target_fingerprint))
            {
                res += cnt[target_fingerprint];
            }

            // 【关键逻辑 3：动态更新】
            // 查完之后，把自己的指纹存入 map，供后续进来的城市进行配对。
            cnt[current_fingerprint]++;
        }
    }

    // 最终输出统计的总对数
    cout << res << endl;

    return 0;
}

/**
 * 【学习总结：Map + 累加思路的精髓】
 *
 * 1. 为什么比 set 强？
 *    - set 只能解决“有没有”，map 才能解决“有多少”。
 *    - 在配对问题中，一个新元素往往能与“一整群”旧元素匹配，必须使用计数累加。
 *
 * 2. 边走边算 (One-pass Algorithm)：
 *    - 我们不需要存下所有的城市再进行双重循环 (O(N^2))。
 *    - 通过一边读取、一边查询 map、一边更新 map，我们将复杂度降到了 O(N log M)。
 *    - 这种方式还天然避免了“重复计算”：
 *      当 A 看到 B 时，res 增加了；等 B 进来时，A 已经在 map 里了，不用担心漏掉。
 *
 * 3. 抽象化的威力：
 *    - 字符串哈希/特征提取的目的，就是把复杂的对象（城市+州）简化成一个可比较的“指纹”。
 *    - 只要指纹相同，在逻辑上它们就是一类“零件”。
 *
 * 4. 这种思路的通用性：
 *    - 求和为 K 的数对：找 target = K - current。
 *    - 求差为 K 的数对：找 target = current + K 或 current - K。
 *    - 本题：找 target = 反转后的指纹。
 */