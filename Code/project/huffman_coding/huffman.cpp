#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>
using namespace std;
struct Node
{
    char data;
    int freq;
    Node *left, *right;
    Node(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};
struct compare
{
    bool operator()(Node *l, Node *r)
    {
        return l->freq > r->freq;
    }
};

int main()
{
    ifstream input;
    input.open("input.txt");
    cout << "open file successfully!" << endl;
    map<char, int> frequency;
    char c;
    while (input.get(c))
    {
        frequency[c]++;
    }
    priority_queue<Node *, vector<Node *>, compare> pq;
    for (auto &pair : frequency)
    {
        Node *new_node = new Node(pair.first, pair.second);
        pq.push(new_node);
    }
    while (pq.size() > 1)
    {
        Node *l = pq.top();
        pq.pop();
        Node *r = pq.top();
        pq.pop();
        Node *new_tree_node = new Node(0, l->freq + r->freq);
        new_tree_node->left = l;
        new_tree_node->right = r;
        pq.push(new_tree_node);
    }

    return 0;
}