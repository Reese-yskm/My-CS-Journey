#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_COUNT 10 // 为了方便观察冲突，我们把数组开小一点

// 1. 定义存储节点
typedef struct Node
{
    int key;           // 键
    int value;         // 值（比如学生学号对应的成绩）
    struct Node *next; // 指向冲突的下一个节点
} Node;

// 2. 定义哈希表主体
typedef struct
{
    Node *buckets[BUCKET_COUNT]; // 桶数组，每个元素是一个链表头指针
} HashTable;

// 3. 简单的哈希函数 (除法取余法)
int hash(int key)
{
    return key % BUCKET_COUNT;
}

// 4. 插入操作 (Insert)
void hash_table_insert(HashTable *ht, int key, int value)
{
    int index = hash(key);

    // 创建新节点
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;

    // 采用“头插法”：新节点指向当前的头，然后新节点变成新的头 (O(1)操作)
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;

    printf("Inserted: Key %d -> Value %d at Index %d\n", key, value, index);
}

// 5. 查找操作 (Search)
int hash_table_search(HashTable *ht, int key)
{
    int index = hash(key);
    Node *current = ht->buckets[index];

    // 在对应的链表中遍历
    while (current != NULL)
    {
        if (current->key == key)
        {
            return current->value; // 找到了，返回对应的值
        }
        current = current->next;
    }
    return -1; // 代表没找到
}

// 6. 删除操作 (Delete) - 这是一个非常好的逻辑练习
void hash_table_delete(HashTable *ht, int key)
{
    int index = hash(key);
    Node *current = ht->buckets[index];
    Node *prev = NULL;

    while (current != NULL)
    {
        if (current->key == key)
        {
            if (prev == NULL)
            {
                // 如果要删的是链表头
                ht->buckets[index] = current->next;
            }
            else
            {
                // 如果在链表中间或尾部
                prev->next = current->next;
            }
            free(current);
            printf("Deleted key %d from Index %d\n", key, index);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// --- 测试 ---
int main()
{
    HashTable ht;
    // 初始化哈希表，将所有桶置为空
    for (int i = 0; i < BUCKET_COUNT; i++)
        ht.buckets[i] = NULL;

    printf("--- Building Hash Table ---\n");
    hash_table_insert(&ht, 1, 95);  // Index 1
    hash_table_insert(&ht, 11, 88); // Index 1 (发生冲突！)
    hash_table_insert(&ht, 21, 70); // Index 1 (再次冲突！)
    hash_table_insert(&ht, 5, 100); // Index 5

    printf("\n--- Searching ---\n");
    int val = hash_table_search(&ht, 11);
    if (val != -1)
        printf("Found Key 11 with Value: %d\n", val);
    else
        printf("Key 11 not found.\n");

    printf("\n--- Deleting ---\n");
    hash_table_delete(&ht, 11);

    val = hash_table_search(&ht, 11);
    if (val != -1)
        printf("Found Key 11 after delete: %d\n", val);
    else
        printf("Key 11 not found after delete.\n");

    return 0;
}