#ifndef __LIST__
#define __LIST__

typedef struct _node
{
    int value;
    struct _node *next;
} Node;

typedef struct _list
{
    Node *head;
    // Node* tail; // 进阶：记录尾指针可将插入效率从 O(n) 提升至 O(1)
} List;

void add(List *plist, int number);
Node *list_search(List *plist, int target);
void list_delete(List *plist, int target);
void list_clear(List *plist);
#endif