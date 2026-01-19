#ifndef _ARRAY_H_
#define _ARRAY_H_

// 定义可变数组结构体
typedef struct
{
    int *array; // 指向堆空间中实际存储数据的数组
    int size;   // 当前数组的逻辑大小
} array;

// 接口函数声明
array array_create(int init_size);           // 创建并初始化数组
void array_free(array *a);                   // 释放数组内存
int array_size(const array *a);              // 获取当前大小（封装）
int *array_at(array *a, int index);          // 访问指定索引（带自动扩容逻辑）
void array_inflate(array *a, int more_size); // 内部函数：底层内存搬迁

#endif // _ARRAY_H_