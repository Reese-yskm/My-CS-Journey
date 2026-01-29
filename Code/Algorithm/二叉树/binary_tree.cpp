#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
    int key;
    Node *left;
    Node *right;
    Node *parent;
    Node(int k) : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
};

void inorder_tree_walk(Node *x)
{
    if (x != NULL)
    {
        inorder_tree_walk(x->left);
        cout << x->key << " ";
        inorder_tree_walk(x->right);
    }
}

Node *tree_search(Node *x, int k)
{
    if (x == NULL || x->key == k)
    {
        return x;
    }
    else if (k < x->key)
    {
        return tree_search(x->left, k);
    }
    else
    {
        return tree_search(x->right, k);
    }
}

Node *iterative_tree_search(Node *x, int k)
{
    while (x != NULL && x->key != k)
    {
        if (k < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    return x;
}

Node *tree_minimum(Node *x)
{
    if (x == nullptr)
        return nullptr; // 安全检查
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

Node *tree_maximum(Node *x)
{
    if (x == nullptr)
        return nullptr; // 安全检查
    while (x->right != NULL)
    {
        x = x->right;
    }
    return x;
}

Node *tree_successor(Node *x)
{
    if (x->right != NULL)
    {
        return tree_minimum(x->right);
    }
    Node *y = x->parent;
    while (y != NULL && x == y->right)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

Node *tree_predecessor(Node *x)
{
    if (x->left != NULL)
    {
        return tree_maximum(x->left);
    }
    Node *y = x->parent;
    while (y != NULL && x == y->left)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

void tree_insert(Node *&root, Node *z)
{
    Node *y = NULL;
    Node *x = root;
    while (x != NULL)
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == NULL)
    {
        root = z;
    }
    else if (z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
}
// 用v来替换u
void transplant(Node *&root, Node *u, Node *v)
{
    if (u->parent == NULL)
    {
        root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != NULL)
    {
        v->parent = u->parent;
    }
}

void tree_delete(Node *&root, Node *z)
{
    if (z->left == nullptr)
    {
        // 情况 A：没有左孩子，用右边顶替
        transplant(root, z, z->right);
    }
    else if (z->right == nullptr)
    {
        // 情况 B：没有右孩子，用左边顶替
        transplant(root, z, z->left);
    }
    else
    {
        // 情况 C & D：有两个孩子
        Node *y = tree_minimum(z->right); // 寻找后继 y

        if (y->parent != z)
        {
            // 情况 D：y 不是 z 的直接孩子
            transplant(root, y, y->right); // 用 y 的右孩子顶替 y
            y->right = z->right;           // z 的右子树交给 y
            y->right->parent = y;
        }

        // 情况 C：用 y 顶替 z
        transplant(root, z, y);
        y->left = z->left; // z 的左子树交给 y
        y->left->parent = y;
    }
    delete z; // 彻底释放内存
}
int main()
{
    Node *root = nullptr;

    // 1. 测试数据 (使用书中经典的例子)
    // 数组序列: 15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9
    vector<int> keys = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};

    printf("--- 1. Building Tree (Inserting keys) ---\n");
    for (int k : keys)
    {
        tree_insert(root, new Node(k));
    }
    printf("Done.\n\n");

    // 2. 测试中序遍历 (验证数据是否有序)
    printf("--- 2. In-order Traversal (Should be sorted) ---\n");
    inorder_tree_walk(root);
    printf("\n\n");

    // 3. 测试最小值和最大值
    printf("--- 3. Min & Max ---\n");
    Node *min_node = tree_minimum(root);
    Node *max_node = tree_maximum(root);
    if (min_node)
        printf("Tree Minimum: %d\n", min_node->key);
    if (max_node)
        printf("Tree Maximum: %d\n", max_node->key);
    printf("\n");

    // 4. 测试查找功能
    printf("--- 4. Search Test ---\n");
    int target = 13;
    Node *found = iterative_tree_search(root, target);
    if (found)
    {
        printf("Key %d found at address %p\n", target, (void *)found);

        // 5. 测试前驱和后继 (这是本章最精妙的部分)
        // 情况 A: 节点 13 没有右子树，寻找它的后继
        printf("\n--- 5. Successor & Predecessor Test ---\n");
        Node *succ = tree_successor(found);
        Node *pred = tree_predecessor(found);

        printf("Node: %d\n", found->key);
        if (pred)
            printf("  Predecessor: %d\n", pred->key);
        else
            printf("  Predecessor: None\n");

        if (succ)
            printf("  Successor: %d\n", succ->key);
        else
            printf("  Successor: None\n");

        // 情况 B: 测试根节点 15 的后继 (它有右子树)
        Node *root_node = iterative_tree_search(root, 15);
        Node *root_succ = tree_successor(root_node);
        printf("Node 15 (Root) Successor: %d\n", root_succ->key);
    }
    else
    {
        printf("Key %d not found!\n", target);
    }
    printf("--- 6. Deletion Test ---\n");
    Node *z = iterative_tree_search(root, 6); // 假设我们要删掉 6
    if (z)
    {
        printf("Deleting node 6...\n");
        tree_delete(root, z);
    }
    printf("In-order traversal after deletion: ");
    inorder_tree_walk(root);
    printf("\n");
    return 0;
}