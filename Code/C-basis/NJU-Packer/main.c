#include "packer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *FILENAME = "data.pack";

/**
 * @brief 将字符串打包写入二进制文件
 * @param s 待写入的字符串
 * @param id 数据块的编号
 */
void pack_data(const char *s, int id)
{
    DataHeader header;
    header.id = id;
    header.length = strlen(s); // 获取字符串长度（不含 \0）

    // 使用 "ab" (Append Binary) 模式：在文件末尾追加，不破坏已有数据
    FILE *fp = fopen(FILENAME, "ab");
    if (fp == NULL)
    {
        perror("Failed to open file for packing");
        return;
    }

    // 第一步：写入固定长度的“面单”（Header）
    fwrite(&header, sizeof(DataHeader), 1, fp);

    // 第二步：紧跟着写入“货物”（载荷数据）
    // 注意：这里用 header.length 而不是 sizeof(s)
    fwrite(s, header.length, 1, fp);

    fclose(fp); // 必须关闭，确保数据从缓冲区刷入硬盘
}

/**
 * @brief 从二进制文件中解析出所有打包的数据
 */
void read_pack()
{
    // 使用 "rb" (Read Binary) 模式：从头开始读取
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL)
    {
        perror("Failed to open file for reading");
        return;
    }

    DataHeader header;
    // 循环读取：尝试读取一个 Header 的大小
    // fread 返回成功读入的项目数。如果返回 < 1，说明读到了文件末尾
    while (fread(&header, sizeof(DataHeader), 1, fp) == 1)
    {
        printf("--- Block Found ---\n");
        printf("ID     : %u\n", header.id);
        printf("Length : %u\n", header.length);

        // 核心逻辑：根据 Header 里的长度，动态向内存申请空间
        // 多申请 1 字节是为了存放 C 语言字符串必备的 '\0'
        char *payload = (char *)malloc(header.length + 1);
        if (payload == NULL)
        {
            fprintf(stderr, "Memory allocation failed!\n");
            break;
        }

        // 将文件中的载荷内容读入刚才申请的内存
        fread(payload, header.length, 1, fp);

        // 【关键】手动补齐结束符。二进制文件通常不存 \0，
        // 但 C 语言的 printf(%s) 必须依赖 \0 才能知道在哪停止。
        payload[header.length] = '\0';

        printf("String : %s\n\n", payload);

        // 释放动态内存，防止内存泄漏（Memory Leak）
        free(payload);
    }

    fclose(fp);
}

int main(void)
{
    // 【环境清理】每次运行前，以 "wb" 模式打开一次，清空旧的 data.pack
    // 这相当于数据库的“初始化”操作
    FILE *fp_clear = fopen(FILENAME, "wb");
    if (fp_clear)
        fclose(fp_clear);

    // 模拟 3 次输入并打包
    for (int i = 1; i <= 3; i++)
    {
        char buffer[101];
        printf("Please enter string #%d (less than 100 chars):\n", i);
        // 使用 %s 读入单词（遇到空格停止）
        if (scanf("%100s", buffer) == 1)
        {
            pack_data(buffer, i);
        }
    }

    printf("\n============================\n");
    printf("Parsing Packed Binary File:\n");
    printf("============================\n");

    // 执行解析
    read_pack();

    return 0;
}