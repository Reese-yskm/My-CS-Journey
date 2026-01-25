#include <stdio.h>
#include <stdlib.h>
void quick_sort(int *a, int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int q = l + (r - l) / 2;
    int i = l - 1;
    int j = r + 1;
    while (i < j)
    {
        do
        {
            i++;
        } while (a[i] < a[q]);
        do
        {
            j--;
        } while (a[j] > a[q]);
        if (i < j)
        {
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    quick_sort(a, l, j);
    quick_sort(a, j + 1, r);
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
    quick_sort(a, 0, size - 1);
    printf("After sorting:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }

    return 0;
}