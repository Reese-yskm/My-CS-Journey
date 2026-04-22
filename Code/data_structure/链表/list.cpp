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
    for (int i = 0; i < k - 1; i++)
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
int main()
{
    vector<int> arr = {4, 7, 2, 5, 1};
    Listnode *my_list = init_List(arr);

    cout << "Original list: ";
    print__each_value(my_list);

    // Check for cycle
    cout << (hasCycle(my_list) ? "Cycle detected" : "No cycle") << "\n";

    // Find and print middle node
    Listnode *middle = findMiddle(my_list);
    if (middle)
    {
        cout << "Middle node: " << middle->val << "\n";
    }
    else
    {
        cout << "List is empty\n";
    }

    // Find and print the 3rd node
    int k = 2;
    Listnode *kth = findKthElementEnd(my_list, k);
    if (kth)
    {
        cout << "The " << k << "th node from end is: " << kth->val << "\n";
    }
    else
    {
        cout << "The list is too short to find the " << k << "the node from end.\n";
    }

    Listnode *reversed_list = reverseList(my_list);
    cout << "Reversed list: ";
    print__each_value(reversed_list);
    return 0;
}