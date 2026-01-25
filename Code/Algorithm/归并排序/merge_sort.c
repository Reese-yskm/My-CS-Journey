#include <stdio.h>
#include <stdlib.h>

void merge(int *a, int p, int q, int r)
{
    int n1 = q - p + 1; // 左子数组长度
    int n2 = r - q;     // 右子数组长度

    // 创建临时数组
    int L[n1], R[n2];

    // 拷贝数据，注意下标对齐
    for (int i = 0; i < n1; i++)
        L[i] = a[p + i];
    for (int j = 0; j < n2; j++)
        R[j] = a[q + 1 + j]; // 从 q+1 开始

    int i = 0; // L 的索引
    int j = 0; // R 的索引
    int k = p; // 原数组的索引

    // 逐个比较并合并，直到其中一个临时数组走完
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            a[k] = L[i];
            i++;
        }
        else
        {
            a[k] = R[j];
            j++;
        }
        k++;
    }

    // 如果 L 还有剩余，拷贝回去
    while (i < n1)
    {
        a[k] = L[i];
        i++;
        k++;
    }

    // 如果 R 还有剩余，拷贝回去
    while (j < n2)
    {
        a[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int *a, int p, int r)
{
    if (p < r)
    {
        int q = p + (r - p) / 2;
        merge_sort(a, p, q);
        merge_sort(a, q + 1, r);
        merge(a, p, q, r);
    }
    return;
}

int main(void)
{
    int a[7] = {3, 5, 2, 1, 4, 7, 6};
    int size = sizeof(a) / sizeof(a[0]);
    printf("Before sorting:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    merge_sort(a, 0, size - 1);
    printf("After sorting:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }

    return 0;
}