#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. 定义一个结构体（回顾之前的结构体知识）
typedef struct
{
    int id;
    char name[20];
    double score;
} Student;

// 函数声明：以十六进制格式打印文件内容（像底层程序员一样观察数据）
void hexDump(const char *filename);

int main(void)
{
    const char *filename = "student.data";

    // --- 第一部分：格式化输入与二进制写入 ---
    Student s1 = {1001, "Alice", 95.5};

    // 以 "wb" (write binary) 模式打开文件
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        perror("无法打开文件进行写入");
        return 1;
    }

    // 使用 fwrite 直接将内存中的结构体“镜像”写入硬盘
    // 参数：数据指针，单个大小，个数，文件流
    fwrite(&s1, sizeof(Student), 1, fp);
    fclose(fp);
    printf("成功将学生数据写入二进制文件: %s\n\n", filename);

    // --- 第二部分：文件定位与二进制读取 ---
    FILE *fp_read = fopen(filename, "rb");
    if (fp_read == NULL)
        return 1;

    // 演示 fseek 和 ftell：测量文件大小
    fseek(fp_read, 0, SEEK_END);    // 移动到文件末尾
    long fileSize = ftell(fp_read); // 获取当前位置（即总长度）
    printf("该二进制文件的总字节数: %ld 字节\n", fileSize);
    rewind(fp_read); // 回到文件头准备读取

    Student s2;
    // 使用 fread 将字节流还原回结构体变量
    if (fread(&s2, sizeof(Student), 1, fp_read) == 1)
    {
        // 使用格式化输出显示结果
        // %.1f 控制小数点，%-10s 控制左对齐
        printf("读取到的数据: ID=%d, Name=%-10s, Score=%.1f\n", s2.id, s2.name, s2.score);
    }
    fclose(fp_read);

    // --- 第三部分：底层视角 (Hex Dump) ---
    printf("\n--- 该文件的底层十六进制镜像 (Hex Dump) ---\n");
    hexDump(filename);

    return 0;
}

// 模拟 Linux 下的 hexdump 工具逻辑
void hexDump(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    unsigned char byte;
    int count = 0;

    // 逐字节读取
    while (fread(&byte, 1, 1, fp) == 1)
    {
        // 格式化输出：以十六进制打印字节，占2位，不足补0
        printf("%02X ", byte);
        count++;
        if (count % 8 == 0)
            printf(" "); // 每8位空一格
        if (count % 16 == 0)
            printf("\n"); // 每16位换行
    }
    printf("\n");
    fclose(fp);
}