#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>   // 必须加
#include <cstdlib> // 必须加

using namespace std;

int partition(vector<int> &a, int l, int r)
{
    // 随机化优化
    int randomIndex = l + rand() % (r - l + 1);
    swap(a[randomIndex], a[r]);

    int pivot = a[r];
    int i = l - 1;
    for (int j = l; j < r; j++)
    {
        if (a[j] <= pivot)
        {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[r]);
    return i + 1;
}

// 迭代版：比递归更稳健，更省内存
int find_kth_iterative(vector<int> &a, int k)
{
    int l = 0, r = a.size() - 1;
    while (l <= r)
    {
        int t = partition(a, l, r);
        if (t == k)
            return a[t];
        else if (k < t)
            r = t - 1;
        else
            l = t + 1;
    }
    return -1; // 理论上不会走到这里
}

int main()
{
    srand(time(0)); // 播种随机数
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n))
        return 0;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    cin >> k;

    // 防御性编程：检查 k 的合法性
    if (k < 0 || k >= n)
    {
        cout << "Error: k is out of bounds!" << endl;
        return 0;
    }

    int res = find_kth_iterative(a, k);
    cout << "The k_th number is: " << res << endl;

    return 0;
}