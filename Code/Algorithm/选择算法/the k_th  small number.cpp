#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int partition(vector<int> &a, int l, int r)
{
    // 随机化：防止遇到有序数组退化成 O(n^2)
    int randomIndex = l + rand() % (r - l + 1);
    swap(a[randomIndex], a[r]);
    int pivot = r;
    int i = l - 1;
    for (int j = l; j < pivot; j++)
    {
        if (a[j] <= a[pivot])
        {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[pivot]);
    return i + 1;
}
int find_kth(vector<int> &a, int l, int r, int k)
{
    int t = partition(a, l, r);
    if (k == t)
    {
        return a[k];
    }
    else if (k < t)
    {
        return find_kth(a, l, t - 1, k);
    }
    else
    {
        return find_kth(a, t + 1, r, k); // 一定注意偏移量的问题
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout << "Please enter the length of the array:\n";
    int n;
    cin >> n;
    vector<int> a(n);
    cout << "Please enter every element:\n";
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    cout << "Please enter the k:\n";
    int k;
    cin >> k;
    int len = a.size();
    int res = find_kth(a, 0, len - 1, k);
    cout << "The k_th number is: " << res << endl;
    return 0;
}