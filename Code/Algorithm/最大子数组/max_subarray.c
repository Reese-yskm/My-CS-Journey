#include <stdio.h>
#include <stdlib.h>

struct result
{
    int left;
    int right;
    int sum;
};
struct result find_max_crossing_subarray(int *a, int low, int mid, int high)
{
    int left_sum = -1e9; // 左侧子数组和
    int sum = 0;
    int max_left; // 左侧子数组左侧下标
    for (int i = mid; i >= low; i--)
    { // 向左找
        sum += a[i];
        if (sum > left_sum)
        {
            left_sum = sum;
            max_left = i;
        }
    }

    int right_sum = -1e9; // 右侧子数组和
    sum = 0;
    int max_right; // 右侧子数组右侧下标
    for (int j = mid + 1; j <= high; j++)
    { // 向右找
        sum += a[j];
        if (sum > right_sum)
        {
            right_sum = sum;
            max_right = j;
        }
    }

    return (struct result){max_left, max_right, left_sum + right_sum};
}

struct result find_maximum_subarray(int *a, int low, int high)
{
    if (low == high)
        return (struct result){low, high, a[low]}; // 递归终点

    int mid = (low + high) / 2;
    // 递归左半边
    struct result left_res = find_maximum_subarray(a, low, mid);
    // 递归右半边
    struct result right_res = find_maximum_subarray(a, mid + 1, high);
    // 处理跨越中点
    struct result cross_res = find_max_crossing_subarray(a, low, mid, high);

    // 返回三者中 sum 最大的那个
    if (left_res.sum >= right_res.sum && left_res.sum >= cross_res.sum)
        return left_res;
    else if (right_res.sum >= left_res.sum && right_res.sum >= cross_res.sum)
        return right_res;
    else
        return cross_res;
}

int main(void)
{
    int a[] = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    int size = sizeof(a) / sizeof(a[0]);
    struct result res = find_maximum_subarray(a, 0, size - 1);
    printf("The maximum subarray is:\n");
    for (int i = res.left; i <= res.right; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    printf("The maximum subarray sum is:%d\n", res.sum);
    return 0;
}