#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// 打印最佳加括号方案的递归函数
void print_optimal_parens(const vector<vector<int>> &s, int i, int j)
{
    if (i == j)
    {
        cout << "A" << i;
    }
    else
    {
        cout << "(";
        print_optimal_parens(s, i, s[i][j]);
        print_optimal_parens(s, s[i][j] + 1, j);
        cout << ")";
    }
}

void matrix_chain_order(const vector<int> &p)
{
    int n = p.size() - 1; // 矩阵个数
    // m[i][j] 存储最小乘法次数
    vector<vector<int>> m(n + 1, vector<int>(n + 1, 0));
    // s[i][j] 存储获得最优解时的分割点 k
    vector<vector<int>> s(n + 1, vector<int>(n + 1, 0));

    // l 是链的长度 (从长度 2 开始)
    for (int l = 2; l <= n; l++)
    {
        for (int i = 1; i <= n - l + 1; i++)
        {
            int j = i + l - 1;
            m[i][j] = INT_MAX;
            // 尝试每一个可能的分割点 k
            for (int k = i; k <= j - 1; k++)
            {
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j])
                {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    cout << "--- Matrix Chain Multiplication DP Test ---" << endl;
    cout << "Minimum number of multiplications: " << m[1][n] << endl;
    cout << "Optimal Parenthesization: ";
    print_optimal_parens(s, 1, n);
    cout << endl;
}

int main()
{
    // 矩阵维数: A1(30x35), A2(35x15), A3(15x5), A4(5x10), A5(10x20), A6(20x25)
    // p 数组存储的是 [30, 35, 15, 5, 10, 20, 25]
    vector<int> p = {30, 35, 15, 5, 10, 20, 25};

    matrix_chain_order(p);

    return 0;
}