#include <stdio.h>
#include <stdlib.h>
void counting_sort(int *a, int n, int k)
{
    int *count = (int *)calloc(k + 1, sizeof(int));
    int *output = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        count[a[i]]++;
    }
    for (int i = 1; i <= k; i++)
    {
        count[i] += count[i - 1];
    }
    for (int i = n - 1; i >= 0; i--)
    {
        output[count[a[i]] - 1] = a[i];
        count[a[i]]--;
    }
    for (int i = 0; i < n; i++)
    {
        a[i] = output[i];
    }
    free(count);
    free(output);
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
    counting_sort(a, size, 7);
    printf("After sorting:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }

    return 0;
}