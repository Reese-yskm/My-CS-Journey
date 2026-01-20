#include <stdio.h>
#include <stdlib.h>

void insertion_sort(int *a, int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int main(void)
{
    int a[5] = {3, 5, 2, 1, 4};
    int size = sizeof(a) / sizeof(a[0]);
    printf("Before sorting:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    insertion_sort(a, size);
    printf("After sorting:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }

    return 0;
}