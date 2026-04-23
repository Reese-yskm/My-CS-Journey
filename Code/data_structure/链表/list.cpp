#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <string>
#include <algorithm>
#include <cmath>
#include <set>
#include <cctype>
#include <map>
#include <iomanip>
using namespace std;
struct Listnode
{
    int val;
    Listnode *next;
    Listnode(int x) : val(x), next(nullptr) {}
};
Listnode *init_List(const vector<int> &a)
{
    if (a.empty())
    {
        return nullptr;
    }

    Listnode *head = new Listnode(a[0]);
    Listnode *cur = head;
    for (int i = 1; i < a.size(); i++)
    {
        cur->next = new Listnode(a[i]);
        cur = cur->next;
    }
    return head;
}
void print__each_value(Listnode *head)
{
    Listnode *cur = head;
    while (cur)
    {
        cout << cur->val << " ";
        cur = cur->next;
    }
    cout << "\n";
}

Listnode *deleteElement(Listnode *head, int x)
{
    Listnode *dummy = new Listnode(0);
    dummy->next = head;
    Listnode *cur = dummy;
    while (cur && cur->next)
    {
        if (cur->next->val == x)
        {
            cur->next = cur->next->next;
        }
        else
        {
            cur = cur->next;
        }
    }
    return dummy->next;
}

Listnode *deleteKthElementEnd(Listnode *head, int k)
{
    Listnode *dummy = new Listnode(0);
    dummy->next = head;
    Listnode *slow = dummy;
    Listnode *quick = dummy;
    for (int i = 0; i < k; i++)
    {
        quick = quick->next;
    }
    while (quick->next)
    {
        slow = slow->next;
        quick = quick->next;
    }
    slow->next = slow->next->next;
    return dummy->next;
}

Listnode *reverseList(Listnode *head)
{
    Listnode *pre = nullptr;
    Listnode *cur = head;
    while (cur)
    {
        Listnode *next = cur->next;
        cur->next = pre;

        pre = cur;
        cur = next;
    }
    return pre;
}
bool hasCycle(Listnode *head)
{
    Listnode *slow = head;
    Listnode *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            return true;
        }
    }
    return false;
}

Listnode *findMiddle(Listnode *head)
{
    Listnode *slow = head;
    Listnode *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
Listnode *findKthElementEnd(Listnode *head, int k)
{
    Listnode *slow = head;
    Listnode *fast = head;
    for (int i = 0; i < k; i++)
    {
        if (fast == nullptr)
        {
            return nullptr;
        }

        fast = fast->next;
    }
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

Listnode *swapPairs(Listnode *head)
{
    Listnode *dummy = new Listnode(0);
    dummy->next = head;

    Listnode *pre = dummy;

    while (pre->next && pre->next->next)
    {
        Listnode *a = pre->next;
        Listnode *b = a->next;
        Listnode *n = b->next;
        pre->next = b;
        a->next = n;
        b->next = a;
        pre = a;
    }

    return dummy->next;
}

Listnode *reverseKGroup(Listnode *head, int k)
{
    Listnode *dummy = new Listnode(0);
    dummy->next = head;
    Listnode *pre = dummy;

    while (true)
    {
        Listnode *end = pre;

        // 1. 找到这一组最后一个节点
        for (int i = 0; i < k; i++)
        {
            end = end->next;
            if (end == nullptr)
                return dummy->next;
        }

        // 2. 记录关键节点
        Listnode *start = pre->next;
        Listnode *nextGroup = end->next;

        // 3. 切断
        end->next = nullptr;

        // 4. 反转并接回去
        pre->next = reverseList(start);
        start->next = nextGroup;

        // 5. 更新 pre
        pre = start;
    }
}

Listnode *mergeTwoOrderedLists(Listnode *l1, Listnode *l2)
{
    Listnode *dummy = new Listnode(0);
    Listnode *cur = dummy;
    while (l1 && l2)
    {
        if (l1->val < l2->val)
        {
            cur->next = l1;
            l1 = l1->next;
        }
        else
        {
            cur->next = l2;
            l2 = l2->next;
        }
        cur = cur->next;
    }
    if (l1)
    {
        cur->next = l1;
    }
    if (l2)
    {
        cur->next = l2;
    }
    return dummy->next;
}

int main()
{
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7};
    vector<int> b = {1, 3, 5};
    vector<int> c = {2, 4, 6};
    Listnode *my_list = init_List(arr);
    Listnode *l1 = init_List(b);
    Listnode *l2 = init_List(c);
    // cout << "Original list: ";
    // print__each_value(my_list);

    // // Check for cycle
    // cout << (hasCycle(my_list) ? "Cycle detected" : "No cycle") << "\n";

    // // Find and print middle node
    // Listnode *middle = findMiddle(my_list);
    // if (middle)
    // {
    //     cout << "Middle node: " << middle->val << "\n";
    // }
    // else
    // {
    //     cout << "List is empty\n";
    // }

    // // Find and print the 3rd node
    // int k = 2;
    // Listnode *kth = findKthElementEnd(my_list, k);
    // if (kth)
    // {
    //     cout << "The " << k << "th node from end is: " << kth->val << "\n";
    // }
    // else
    // {
    //     cout << "The list is too short to find the " << k << "the node from end.\n";
    // }

    // int x = 7;
    // cout << "Delete the val " << x << " :";
    // print__each_value(deleteElement(my_list, x));

    // cout << "Delete the 3th node from end: ";
    // print__each_value(deleteKthElementEnd(my_list, 2));

    // Listnode *reversed_list = reverseList(my_list);
    // cout << "Reversed list: ";
    // print__each_value(reversed_list);

    // cout << "swap in pair: ";
    // print__each_value(swapPairs(my_list));

    // cout << "swap in k group: ";
    // print__each_value(reverseKGroup(my_list, 3));

    cout << "After merge: ";
    print__each_value(mergeTwoOrderedLists(l1, l2));
    return 0;
}