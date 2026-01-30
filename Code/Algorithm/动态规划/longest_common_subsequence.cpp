#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// print the lcs
void print_lcs(const vector<vector<char>> &b, string X, int i, int j)
{
    if (i == 0 || j == 0)
    {
        return;
    }
    if (b[i][j] == 'D')
    {
        print_lcs(b, X, i - 1, j - 1);
        cout << X[i - 1]; // be careful about the index, it needs to minus 1
    }
    else if (b[i][j] == 'U')
    {
        print_lcs(b, X, i - 1, j);
    }
    else
    {
        print_lcs(b, X, i, j - 1);
    }
}
// calculate the length of lcs
void lcs_length(string X, string Y)
{
    int n = X.length();
    int m = Y.length();
    // array c record the length,array b record the direction
    vector<vector<int>> c(n + 1, vector<int>(m + 1, 0));
    vector<vector<char>> b(n + 1, vector<char>(m + 1));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (X[i - 1] == Y[j - 1])
            {
                c[i][j] = 1 + c[i - 1][j - 1];
                b[i][j] = 'D';
            }
            else if (c[i - 1][j] > c[i][j - 1])
            {
                c[i][j] = c[i - 1][j];
                b[i][j] = 'U';
            }
            else
            {
                c[i][j] = c[i][j - 1];
                b[i][j] = 'L';
            }
        }
    }
    cout << "--- Longest Common Subsequence DP Test ---" << endl;
    cout << "Length of LCS: " << c[n][m] << endl;
    cout << "LCS Result: ";
    print_lcs(b, X, n, m);
    cout << endl;
}

int main()
{
    string X = "ABCBDABCDS";
    string Y = "BDCABADFS";

    lcs_length(X, Y);

    return 0;
}