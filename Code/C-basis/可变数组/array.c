#include "array.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * 创建数组
 * @param init_size 初始分配的大小
 */
array array_create(int init_size)
{
    array a;
    a.size = init_size;
    a.array = (int *)malloc(sizeof(int) * init_size);

    // [健壮性检查] 检查内存是否申请成功
    if (a.array == NULL)
    {
        fprintf(stderr, "Error: malloc failed in array_create\n");
        exit(1);
    }
    return a;
}

/**
 * 释放资源
 */
void array_free(array *a)
{
    if (a->array != NULL)
    {
        free(a->array);  // 释放堆空间
        a->array = NULL; // 防御性编程：防止野指针
    }
    a->size = 0;
}

/**
 * 获取数组大小
 */
int array_size(const array *a)
{
    return a->size;
}

/**
 * 访问数组元素（核心逻辑）
 * @return 返回指向该元素的指针，使其既可以做左值也可以做右值
 */
int *array_at(array *a, int index)
{
    // [修复逻辑] 如果 index 超过当前大小，则进行扩容
    if (index >= a->size)
    {
        // 计算至少需要再增加多少空间才能装下当前的 index
        int target_size = index + 1;
        array_inflate(a, target_size - a->size);
    }
    // index 小于 0 的防御处理（可选）
    if (index < 0)
    {
        fprintf(stderr, "Error: Index out of bounds\n");
        exit(1);
    }
    return &(a->array[index]);
}

/**
 * 数组扩容（内存搬迁）
 */
void array_inflate(array *a, int more_size)
{
    int new_size = a->size + more_size;
    // 1. 申请新领地
    int *p = (int *)malloc(sizeof(int) * new_size);

    if (p == NULL)
    {
        fprintf(stderr, "Error: malloc failed in array_inflate\n");
        exit(1);
    }

    // 2. 搬家（将旧数据拷贝到新空间）
    for (int i = 0; i < a->size; i++)
    {
        p[i] = a->array[i];
    }

    // 3. 拆除旧房子
    free(a->array);

    // 4. 更新户口本信息
    a->array = p;
    a->size = new_size;

    // 调试信息（学习时可以开启）
    // printf("--- Array inflated to size %d ---\n", a->size);
}

// ================= 测试主函数 =================
int main(void)
{
    array a = array_create(5);
    printf("Initial size: %d\n", array_size(&a));

    // 1. 初始化原有的空间
    for (int i = 0; i < array_size(&a); i++)
    {
        *array_at(&a, i) = 0;
    }

    // 2. 测试“跳跃式”赋值（触发自动扩容）
    printf("Setting index 15 to 99...\n");
    *array_at(&a, 15) = 99; // 原大小只有 5，这里会自动扩到至少 16

    // 3. 打印当前数组内容
    printf("Current size: %d\n", array_size(&a));
    for (int i = 0; i < array_size(&a); i++)
    {
        printf("%d ", *array_at(&a, i));
    }
    printf("\n");

    array_free(&a);
    return 0;
}