/**
 * @file P2367_Difference_Array.cpp
 * @brief 洛谷 P2367 语文成绩 - 差分数组入门
 *
 * 【题目核心】
 * 1. 初始状态：给定 N 个学生的成绩。
 * 2. 区间修改：进行 P 次操作，每次给 [x, y] 区间的学生增加 z 分。
 * 3. 目标：所有操作结束后，求全班的最低分。
 *
 * 【建模思考：为什么暴力会 TLE？】
 * - 暴力做法：每次修改都用 for 循环遍历 [x, y]，复杂度 O(P * N)。
 * - 本题数据：5*10^6 * 5*10^6 = 2.5*10^13，计算机 1 秒绝对跑不完。
 * - 解决方案：差分数组。将区间修改降阶为 O(1) 的单点修改。
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 【关键知识点：什么是差分数组？】
 *
 * 1. 定义：
 *    设原数组为 a，差分数组为 d。
 *    d[i] = a[i] - a[i-1] (规定 a[0] = 0)
 *
 * 2. 性质：
 *    原数组 a[i] 实际上就是差分数组 d 的前缀和：
 *    a[i] = d[1] + d[2] + ... + d[i]
 *
 * 3. 核心威力：区间修改 -> 单点修改
 *    要在原数组区间 [L, R] 全部增加 V：
 *    - 只需要在差分数组执行：d[L] += V
 *    - 并在差分数组执行：d[R+1] -= V
 *    这样，当你求前缀和还原 a 数组时，L 及其之后的数都自动加上了 V，
 *    而 R+1 及其之后的数因为又减去了 V，刚好抵消。
 */

int main()
{
    // 【性能优化】：面对 5*10^6 级别的数据，必须关闭同步流
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    if (!(cin >> n >> p))
        return 0;

    // 差分数组，多开两位防止 [y+1] 越界
    // 注意：全线使用 int 即可，成绩和增量都不大
    vector<int> diff(n + 2, 0);

    // 1. 处理初始成绩：将原始数据转化为差分格式
    // 技巧：也可以把初始成绩看作是在 [i, i] 区间增加了 score 分
    int last_score = 0;
    for (int i = 1; i <= n; i++)
    {
        int current_score;
        cin >> current_score;
        diff[i] = current_score - last_score;
        last_score = current_score;
    }

    // 2. 核心操作：执行 P 次区间修改
    while (p--)
    {
        int x, y, z;
        cin >> x >> y >> z;
        diff[x] += z;     // 左端点加
        diff[y + 1] -= z; // 右端点后一个位置减
    }

    // 3. 收网：还原成绩并动态求最小值
    int min_score = 1e9;        // 初始化为极大值
    int current_real_score = 0; // 用来累加前缀和，还原当前学生的真实分数

    for (int i = 1; i <= n; i++)
    {
        current_real_score += diff[i]; // 前缀和累加
        if (current_real_score < min_score)
        {
            min_score = current_real_score;
        }
    }

    cout << min_score << endl;

    return 0;
}

/**
 * 【避坑指南与深度复盘】
 *
 * 1. 逻辑混淆（我最初犯的错）：
 *    - 错误做法：在已经读入的原始成绩 a[i] 上直接执行 a[x]+=z, a[y+1]-=z。
 *    - 结果：这样混淆了“差分值”和“原始值”。还原时会产生指数级的错误累加。
 *    - 修正：必须明确差分数组是从 0 开始通过修改构建出来的，或者先通过 a 数组计算出正确的 d 数组。
 *
 * 2. 空间管理：
 *    - vector<int>(5e6) 占用约 20MB 内存，题目给 125MB，绰绰有余。
 *    - 在大厂面试中，如果你能提到“差分数组不仅省时间，还能在某些场景下省空间”，是加分项。
 *
 * 3. 翻译能力练习：
 *    - “给 [x, y] 每人加 z 分” -> 翻译为 `d[x]+=z; d[y+1]-=z;`
 *    - “最后看成绩” -> 翻译为 `sum = 0; sum += d[i];`
 *    这种“一对一”的逻辑映射，就是算法翻译能力的本质。
 */