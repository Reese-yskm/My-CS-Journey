/**
 * ======================================================================================
 * @file    P1308_Word_Count.cpp
 * @author  Reese_fan (NJU-CS)
 * @date    2026-03-02
 * @brief   洛谷 P1308 统计单词数 - 字符串流处理与不区分大小写匹配
 * ======================================================================================
 *
 * 【题目分析】
 * 1. 核心需求：在一个长字符串中查找完全匹配的特定单词。
 * 2. 约束条件：
 *    - 不区分大小写。
 *    - 要求完全匹配（不能是子串）。
 *    - 统计次数及第一次出现的首字母位置。
 *
 * ======================================================================================
 * 【算法建模：流式处理 (Stream Processing)】
 *
 * 1. 输入处理：使用 cin.get(c) 逐字符读取，精准捕捉空格位置。
 * 2. 状态机思想：
 *    - 当字符为字母时：不断填入 buffer。
 *    - 当字符非字母（边界）时：将 buffer 与目标单词对比，结算状态，清空 buffer。
 * 3. 匹配技巧：预先将目标单词转为小写，比较时仅需转换当前 buffer。
 * ======================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// 统一转小写的辅助函数
void toLower(string &s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string target;
    if (!(cin >> target))
        return 0;
    toLower(target); // 预处理：只转换一次目标单词，提高效率

    // 清掉第一行后的换行符，防止干扰 cin.get
    string dummy;
    getline(cin, dummy);

    string buffer = "";
    char c;
    int index = 0;      // 当前字符在文章中的位置
    int first_pos = -1; // 第一次出现的起始位置
    int count = 0;      // 出现次数

    // 逐字符读取文章（包括空格）
    while (cin.get(c))
    {
        if (isalpha(c))
        {
            buffer += c;
        }
        else
        {
            // 遇到非字母，说明一个单词结束了，开始结算
            if (!buffer.empty())
            {
                string temp = buffer;
                toLower(temp);
                if (temp == target)
                {
                    count++;
                    if (first_pos == -1)
                    {
                        // 第一次找到时，位置 = 当前索引 - 单词长度
                        first_pos = index - buffer.size();
                    }
                }
                buffer = ""; // 清空缓存
            }
        }
        index++;
    }

    // 最后的扫尾逻辑：防止文章末尾没有空格导致最后一个单词没结算
    if (!buffer.empty())
    {
        toLower(buffer);
        if (buffer == target)
        {
            count++;
            if (first_pos == -1)
                first_pos = index - buffer.size();
        }
    }

    if (count > 0)
    {
        cout << count << " " << first_pos << endl;
    }
    else
    {
        cout << -1 << endl;
    }

    return 0;
}

/**
 * 【反思与感悟】
 *
 * 1. 独立开发的快感：
 *    这道题是我自己独立 AC 的。我深刻体会到：零基础阶段，痛苦的独立死磕才是提升代码能力的
 *    唯一捷径。依靠别人减少的只是暂时的痛苦，而独立思考换来的是长久的直觉。
 *
 * 2. 细节魔鬼：
 *    - 空格处理：cin.get() 与 cin >> 的区别是决定成败的关键。
 *    - 完全匹配：通过“遇到非字母才结算”保证了不会匹配到子串。
 *
 * 3. 职业关联：
 *    这种单词搜索逻辑是文本编辑器（如 VS Code）底层检索功能的基础。在大规模数据下，
 *    这种 O(N) 的流式处理比频繁截取子串要节省大量内存。
 */