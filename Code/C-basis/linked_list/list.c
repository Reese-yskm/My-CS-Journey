#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

void add(List *plist, int number)
{
    // 准备新节点
    Node *p = (Node *)malloc(sizeof(Node));
    p->value = number;
    p->next = NULL;

    // 寻找尾部
    Node *last = plist->head;
    if (last)
    {
        while (last->next)
        {
            last = last->next;
        }
        // 挂接
        last->next = p;
    }
    else
    {
        // 特殊情况
        plist->head = p;
    }
}

Node *list_search(List *plist, int target)
{
    bool isfound = false;
    for (Node *p = plist->head; p; p = p->next)
    {
        if (p->value == target)
        {
            return p;
        }
    }
    return NULL;
}

void list_delete(List *plist, int target)
{
    Node *p, *q;
    for (p = plist->head, q = NULL; p; q = p, p = p->next)
    {
        if (p->value == target)
        {
            if (q)
            {
                q->next = p->next; // 绕过p
            }
            else
            {
                plist->head = p->next; // 删的是头
            }
            free(p); // 物理删除
            break;
        }
    }
}

void list_clear(List *plist)
{
    Node *p = plist->head;
    while (p)
    {
        Node *next = p->next;
        free(p);
        p = next;
    }
    plist->head = NULL;
}

int main(void)
{
    List my_tasks = {NULL}; // 初始化空链表

    // 1. 模拟加入任务
    add(&my_tasks, 101);
    add(&my_tasks, 102);
    add(&my_tasks, 103);
    printf("Init tasks: 101, 102, 103\n");

    // 2. 测试搜索
    Node *found = list_search(&my_tasks, 102);
    if (found)
    {
        printf("Found task: %d at %p\n", found->value, found);
    }

    // 3. 测试删除
    printf("Deleting 102...\n");
    list_delete(&my_tasks, 102);

    // 4. 遍历打印结果
    printf("Current tasks: ");
    for (Node *p = my_tasks.head; p; p = p->next)
    {
        printf("%d ", p->value);
    }
    printf("\n");

    return 0;
}