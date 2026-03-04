/**
 * @file P1597_Pascal_Parser.cpp
 * @author Reese_fan (NJU-CS)
 * @brief 洛谷 P1597 语句解析 - 利用 scanf 模式匹配实现极简解析
 * @date 2026-03-04
 *
 * [心法总结]
 * 1. 模式匹配：当输入具有高度规律的模板时，scanf 的格式串是处理数据的“手术刀”。
 * 2. 字符映射：利用 v[cur - 'a'] 将字符变量映射到数组下标，是处理连续字母变量的经典技巧。
 * 3. 混合编程：在 C++ 中合理利用 C 语言的 scanf 能够大幅减少字符串切片的逻辑负担。
 */

#include <iostream>
#include <cstdio> // scanf 所在的头文件
#include <cctype> // isalpha 所在的头文件

using namespace std;

int main()
{
    // 存储 a, b, c 的值，初始化为 0
    int v[3] = {0, 0, 0};
    char cur, val;

    /**
     * scanf 的精妙之处：
     * 1. 匹配格式 "%c:=%c;"，自动跳过其中的 ":=" 和 ";"。
     * 2. 返回值 2 表示成功读取了两个字符项。
     * 3. 警告：如果输入中有空格，建议写成 " %c:=%c;" (前面加空格) 以自动跳过空白符。
     */
    while (scanf("%c:=%c;", &cur, &val) == 2)
    {
        int right_hand_value;

        // 判断右值是变量还是数字
        if (isalpha(val))
        {
            right_hand_value = v[val - 'a'];
        }
        else
        {
            right_hand_value = val - '0';
        }

        // 执行赋值：左变量下标映射
        v[cur - 'a'] = right_hand_value;
    }

    // 按照题目要求输出 a, b, c 的最终值
    printf("%d %d %d\n", v[0], v[1], v[2]);

    return 0;
}