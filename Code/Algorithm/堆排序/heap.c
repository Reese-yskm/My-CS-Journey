#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // 用于使用 INT_MIN

// --- 核心工具：维护最大堆性质 (下沉) ---
void MAX_HEAPIFY(int *a, int n, int i)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;

    if (left < n && a[left] > a[i])
        largest = left;
    if (right < n && a[right] > a[largest])
        largest = right;

    if (largest != i)
    {
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        MAX_HEAPIFY(a, n, largest);
    }
}

// --- 核心工具：建堆 ---
void BUILD_MAX_HEAP(int *a, int n)
{
    for (int i = (n - 1) / 2; i >= 0; i--)
        MAX_HEAPIFY(a, n, i);
}

// --- 算法 1：堆排序 (原地排序) ---
void HEAPSORT(int *a, int n)
{
    BUILD_MAX_HEAP(a, n);
    for (int i = n - 1; i >= 1; i--)
    {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        MAX_HEAPIFY(a, i, 0); // 剩余堆的大小为 i
    }
}

// --- 优先队列操作 1：查看最大值 ---
int HEAP_MAXIMUM(int *a)
{
    return a[0];
}

// --- 优先队列操作 2：弹出最大值 (出队) ---
int HEAP_EXTRACT_MAX(int *a, int *n)
{
    if (*n < 1)
    {
        printf("Error: Heap underflow!\n");
        return -1;
    }
    int max = a[0];
    a[0] = a[*n - 1]; // 将末尾元素移至根部
    (*n)--;           // 堆大小减 1
    MAX_HEAPIFY(a, *n, 0);
    return max;
}

// --- 优先队列操作 3：提升优先级 (上浮) ---
void HEAP_INCREASE_KEY(int *a, int i, int key)
{
    if (key < a[i])
    {
        printf("Error: New key is smaller than current key!\n");
        return;
    }
    a[i] = key;
    // 只要当前节点大于其父节点，则向上交换
    while (i > 0 && a[(i - 1) / 2] < a[i])
    {
        int temp = a[i];
        a[i] = a[(i - 1) / 2];
        a[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// --- 优先队列操作 4：插入新元素 (入队) ---
void MAX_HEAP_INSERT(int *a, int *n, int key)
{
    (*n)++;
    a[*n - 1] = INT_MIN;               // 先设置为最小值
    HEAP_INCREASE_KEY(a, *n - 1, key); // 再提升为目标值
}

// 辅助函数：打印数组
void print_array(int *a, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

// --- 测试主函数 ---
int main(void)
{
    // Test 1: Heap Sort
    int data[7] = {3, 5, 2, 1, 4, 7, 6};
    int n = 7;
    printf("--- 1. Heap Sort Test ---\n");
    printf("Original array: ");
    print_array(data, n);
    HEAPSORT(data, n);
    printf("Sorted array:   ");
    print_array(data, n);
    printf("\n");

    // Test 2: Priority Queue Dynamic Operations
    printf("--- 2. Priority Queue Test ---\n");
    int heap[100];
    int heap_size = 0;

    printf("Inserting tasks with priorities: 10, 20, 5, 15\n");
    MAX_HEAP_INSERT(heap, &heap_size, 10);
    MAX_HEAP_INSERT(heap, &heap_size, 20);
    MAX_HEAP_INSERT(heap, &heap_size, 5);
    MAX_HEAP_INSERT(heap, &heap_size, 15);
    printf("Current heap: ");
    print_array(heap, heap_size);

    printf("Extracted max priority task: %d\n", HEAP_EXTRACT_MAX(heap, &heap_size));
    printf("Heap after extraction: ");
    print_array(heap, heap_size);

    printf("Increasing key of element at index 2 (value %d) to 30\n", heap[2]);
    HEAP_INCREASE_KEY(heap, 2, 30);
    printf("Heap after increase-key: ");
    print_array(heap, heap_size);

    printf("Extracted max priority task again: %d\n", HEAP_EXTRACT_MAX(heap, &heap_size));
    printf("Final heap state: ");
    print_array(heap, heap_size);

    return 0;
}