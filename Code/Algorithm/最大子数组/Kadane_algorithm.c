#include <stdlib.h>
#include <stdio.h>

int kadane(int *arr, int n)
{
    int max_current = arr[0]; // 以当前位置 arr[i] 结尾的最大子数组之和。
    int max_global = arr[0];  // 全局最大子数组之和。

    for (int i = 1; i < n; i++)
    {
        max_current = (arr[i] > (max_current + arr[i])) ? arr[i] : (max_current + arr[i]);
        if (max_current > max_global)
        {
            max_global = max_current;
        }
    }

    return max_global;
}

int main()
{
    int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    int max_sum = kadane(arr, n);
    printf("Maximum contiguous subarray sum is %d\n", max_sum);
    return 0;
}