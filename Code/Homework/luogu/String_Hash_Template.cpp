/**
 * @file String_Hash_Template.cpp
 * @brief 字符串哈希 (String Hashing) 模板
 *
 * 【核心逻辑】
 * 1. 秦九韶算法应用：将字符串看作一个 P 进制的大整数。
 * 2. 自动取模：使用 unsigned long long (ULL)，溢出时相当于对 2^64 取模。
 * 3. 基数选择：P 通常选 131 或 13331。
 *
 * 【代码流程】
 * 1. 读入字符串 S。
 * 2. ULL hash_val = 0;
 *    for (char c : S) hash_val = hash_val * 131 + c;
 * 3. s.insert(hash_val);
 * 4. 输出 s.size();
 */

#include <iostream>
#include <string>
#include <set>

using namespace std;

typedef unsigned long long ull;

int main()
{
    int n;
    cin >> n;
    set<ull> hashes; // 存储计算出的指纹

    while (n--)
    {
        string s;
        cin >> s;

        ull h = 0;
        // 秦九韶算法/多项式滚动哈希
        for (char c : s)
        {
            h = h * 131 + c;
        }
        hashes.insert(h);
    }

    cout << hashes.size() << endl;

    return 0;
}