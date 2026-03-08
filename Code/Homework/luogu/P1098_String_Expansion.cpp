/**
 * ======================================================================================
 * @file    P1098_String_Expansion.cpp
 * @author  Reese_fan (NJU-CS)
 * @date    2026-03-08
 * @brief   洛谷 P1098 [NOIP 2007 提高组] 字符串的展开 - 经典大模拟
 * ======================================================================================
 *
 * 【题目复盘：模拟题的防错心法】
 *
 * 1. 只读不写原则：
 *    面对需要把一个字符替换为一串字符的题目，不要尝试用 string.replace 或在原串修改。
 *    最稳妥的做法是建立一个结果字符串(res)或者直接输出。
 *
 * 2. 边界检查：
 *    减号 '-' 出现在首尾是不能展开的。检查 s[i-1] 和 s[i+1] 前必须确保 i > 0 且 i < len-1。
 *
 * 3. 逻辑分层：
 *    - 判定层：是否符合展开条件（同类字符且右 > 左）。
 *    - 顺序层 (p3)：决定循环的方向。
 *    - 内容层 (p1)：决定填充的内容。
 *    - 重复层 (p2)：执行重复填充。
 *
 * ======================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int p1, p2, p3;
    string s;
    if (!(cin >> p1 >> p2 >> p3 >> s))
        return 0;

    for (int i = 0; i < s.length(); i++)
    {
        // 当前字符不是减号，直接输出
        if (s[i] != '-')
        {
            cout << s[i];
            continue;
        }

        // 减号是首尾字符，直接输出
        if (i == 0 || i == s.length() - 1)
        {
            cout << '-';
            continue;
        }

        char left = s[i - 1];
        char right = s[i + 1];

        // 判定条件：1.左右同为字母或数字 2.右边严格大于左边
        bool both_digit = isdigit(left) && isdigit(right);
        bool both_lower = islower(left) && islower(right);

        if ((both_digit || both_lower) && right > left)
        {
            // 符合展开条件！开始构造中间内容

            // 确定循环方向 (p3)
            if (p3 == 1)
            { // 顺序
                for (char c = left + 1; c < right; c++)
                {
                    // 确定填充内容 (p1)
                    char fill = c;
                    if (p1 == 2 && isalpha(c))
                        fill = toupper(c); // 转大写
                    if (p1 == 3)
                        fill = '*'; // 转星号

                    // 确定重复次数 (p2)
                    for (int k = 0; k < p2; k++)
                        cout << fill;
                }
            }
            else
            { // 逆序 (p3 == 2)
                for (char c = right - 1; c > left; c--)
                {
                    char fill = c;
                    if (p1 == 2 && isalpha(c))
                        fill = toupper(c);
                    if (p1 == 3)
                        fill = '*';

                    for (int k = 0; k < p2; k++)
                        cout << fill;
                }
            }
        }
        else
        {
            // 不符合条件（比如 a-a, 3-1, a-3），保留减号
            cout << '-';
        }
    }

    cout << endl;
    return 0;
}

/**
 * 【学长深度思考：从模拟到工程设计】
 *
 * 1. 为什么这题是“提高组”？
 *    算法虽然只是简单的循环，但它考察了极其严密的分类讨论能力。
 *    比如：数字展开时，p1=2 依然是数字（toupper对数字无效），这些小点都是丢分点。
 *
 * 2. 代码可读性：
 *    我将 p3 的判断放在外面，内部写重复的 p1 逻辑，虽然代码行数多了，但逻辑链路极短。
 *    在考试中，清晰的逻辑比精简的代码更容易 Debug。
 *
 * 3. 关于 toupper()：
 *    属于 <cctype>。作为南大选手，养成使用库函数而不是手动 +-32 的习惯，
 *    不仅是为了美观，更是为了防止在复杂的 Unicode 环境下翻车。
 *
 * 4. 总结感悟：
 *    面对复杂逻辑，不要怕写“笨代码”。把三层循环套在一起，每一层只负责一个参数的变化，
 *    这种解耦思想能帮你 AC 掉几乎所有的“大模拟”。
 */