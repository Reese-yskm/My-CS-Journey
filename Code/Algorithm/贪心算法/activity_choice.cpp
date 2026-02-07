#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Activity
{
    int id;
    int start;
    int finish;
};

// Sort activities by finish time (Greedy Choice Property)
bool compareFinish(Activity a, Activity b)
{
    return a.finish < b.finish;
}

void activitySelection(vector<Activity> &acts)
{
    // 1. Sort by finish time: O(n log n)
    sort(acts.begin(), acts.end(), compareFinish);

    cout << "--- Selected Activities ---" << endl;

    // 2. The first activity is always part of an optimal solution
    int n = acts.size();
    if (n == 0)
        return;

    cout << "Activity ID: " << acts[0].id << " (" << acts[0].start << ", " << acts[0].finish << ")" << endl;

    int lastFinish = acts[0].finish;

    // 3. Greedily pick the next compatible activity
    for (int i = 1; i < n; i++)
    {
        if (acts[i].start >= lastFinish)
        {
            cout << "Activity ID: " << acts[i].id << " (" << acts[i].start << ", " << acts[i].finish << ")" << endl;
            lastFinish = acts[i].finish;
        }
    }
}

int main()
{
    vector<Activity> acts = {
        {1, 1, 4}, {2, 3, 5}, {3, 0, 6}, {4, 5, 7}, {5, 3, 9}, {6, 5, 9}, {7, 6, 10}, {8, 8, 11}, {9, 8, 12}, {10, 2, 14}, {11, 12, 16}};

    activitySelection(acts);
    return 0;
}