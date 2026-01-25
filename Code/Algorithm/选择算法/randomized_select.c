#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 交换函数
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// 标准分区函数 (Lomuto 分区)
int partition(int *a, int low, int high)
{
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (a[j] <= pivot)
        {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[high]);
    return i + 1;
}

// 随机分区函数：防止最坏情况
int randomized_partition(int *a, int low, int high)
{
    int i = low + rand() % (high - low + 1);
    swap(&a[i], &a[high]);
    return partition(a, low, high);
}

// 核心函数：找到第 k 小的元素 (k 从 0 开始)
int randomized_select(int *a, int p, int r, int i)
{
    if (p == r)
        return a[p];

    // 划分并获取基准的排名
    int q = randomized_partition(a, p, r);
    int k = q - p; // 基准左侧元素个数

    if (i == k)
    {
        return a[q]; // 找到了
    }
    else if (i < k)
    {
        // 在左侧查找
        return randomized_select(a, p, q - 1, i);
    }
    else
    {
        // 在右侧查找，注意修正目标排名
        return randomized_select(a, q + 1, r, i - k - 1);
    }
}

int main(void)
{
    srand(time(NULL)); // 初始化随机种子

    int data[] = {3, 2, 9, 0, 7, 5, 4, 8, 6, 1};
    int n = sizeof(data) / sizeof(data[0]);

    // 假设我们要找第 4 小的数 (即下标为 3)
    int target_rank = 3;

    printf("--- Randomized Select Test ---\n");
    printf("Original array: ");
    for (int j = 0; j < n; j++)
        printf("%d ", data[j]);
    printf("\n");

    int result = randomized_select(data, 0, n - 1, target_rank);

    printf("The element with rank %d is: %d\n", target_rank, result);

    return 0;
}