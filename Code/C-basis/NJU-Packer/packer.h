#ifndef _HEADER_PACKER_H_
#define _HEADER_PACKER_H_

#include <stdint.h>

/**
 * @brief 数据包头结构体
 *
 * 这是一个典型的“元数据”结构。
 * uint32_t 确保在任何机器上 ID 和 Length 都占用精确的 4 字节，
 * 这样打包出的文件才具有可移植性（跨平台兼容）。
 */
typedef struct
{
    uint32_t id;     // 数据块的唯一标识
    uint32_t length; // 紧随其后的有效载荷（Payload）长度
} DataHeader;

// 函数声明
void pack_data(const char *s, int id);
void read_pack();

#endif // _HEADER_PACKER_H_