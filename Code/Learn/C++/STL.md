# C++ STL 算法竞赛/自学详细笔记

## 1. 概述 (Overview)
STL 主要由四部分组成：
*   **容器 (Containers)**：各种数据结构的实现。
*   **算法 (Algorithms)**：排序、查找等。
*   **迭代器 (Iterators)**：连接容器与算法的“桥梁”（行为类似指针）。
*   **适配器 (Adapters)**：对基础容器的封装（如栈、队列）。

---

## 2. 序列容器 (Sequence Containers)

### 2.1 vector（动态数组）
*   **底层**：自动扩容的连续数组。
*   **手写对应**：类似于你之前在归并排序和堆排序中传递的数组，但它支持 `push_back` 自动扩容。
*   **核心操作**：
    ```cpp
    vector<int> v;
    v.push_back(10); // 末尾添加 O(1)
    v.pop_back();    // 末尾删除 O(1)
    v.size();        // 返回大小
    v.clear();       // 清空
    sort(v.begin(), v.end()); // 排序
    ```

### 2.2 list（双向链表）
*   **底层**：双向循环链表。
*   **手写对应**：**《算法导论》第 10.2 节**。
*   **特点**：不支持随机访问（不能 `v[i]`），但在任意位置插入删除都是 $O(1)$。

---

## 3. 容器适配器 (Container Adapters)

### 3.1 stack（栈）
*   **底层**：默认基于 `deque` 实现。
*   **手写对应**：**《算法导论》第 10.1 节**。
*   **操作**：`s.push()`, `s.pop()`, `s.top()`, `s.empty()`。

### 3.2 queue（队列）
*   **底层**：默认基于 `deque` 实现。
*   **手写对应**：**《算法导论》第 10.1 节**。
*   **操作**：`q.push()`, `q.pop()`, `q.front()`, `q.back()`。

### 3.3 priority_queue（优先队列/堆） —— **重要！**
*   **底层**：**《算法导论》第 6 章 - 二叉堆**。
*   **特点**：自动维护最值。
*   **用法转换**：
    ```cpp
    // 默认大顶堆（Max-Heap）
    priority_queue<int> max_heap;
    
    // 小顶堆（Min-Heap），对应合并果子题目
    priority_queue<int, vector<int>, greater<int>> min_heap;
    
    min_heap.push(val); // 插入 O(log n)
    min_heap.top();     // 获取堆顶 O(1)
    min_heap.pop();     // 弹出堆顶 O(log n)
    ```

---

## 4. 关联容器 (Associative Containers)

### 4.1 set / map（有序集合/映射）
*   **底层**：**《算法导论》第 13 章 - 红黑树 (Red-Black Tree)**。
*   **特点**：
    *   内部元素始终**自动排序**。
    *   插入、删除、查找均为 **$O(\log n)$**。
*   **用法**：
    ```cpp
    map<string, int> scores;
    scores["Alice"] = 95; // 插入或修改
    if (scores.count("Alice")) // 查找是否存在
        cout << scores["Alice"];
    ```

### 4.2 unordered_set / unordered_map（哈希表）
*   **底层**：**《算法导论》第 11 章 - 散列表 (Hash Table)**（采用拉链法解决冲突）。
*   **特点**：
    *   不排序，元素乱序。
    *   查询速度平均为 **$O(1)$**。
*   **用法**：与 `map` 几乎一致，但性能通常更高（除非数据被故意构造导致冲突）。

---

## 5. 核心算法库 (Algorithms)

这些函数直接操作容器的迭代器：`v.begin()` 到 `v.end()`。

### 5.1 排序与查找
*   **`std::sort(p, r)`**：
    *   **手写对应**：**《算法导论》第 7 章 - 快速排序**。
    *   **实现**：内省排序（Introsort），结合了快排、堆排和插入排序，保证最坏 $O(n \log n)$。
*   **`std::nth_element(p, k, r)`**：
    *   **手写对应**：**《算法导论》第 9 章 - 快速选择算法 (Quickselect)**。
    *   **功能**：在 $O(n)$ 时间内找到第 $k$ 小的数，并把它放到正确位置。
*   **`std::lower_bound` / `std::upper_bound`**：
    *   **底层**：**二分查找**。要求容器已排序。
    *   `lower_bound`：找第一个 $\ge$ 目标的迭代器。

### 5.2 常用小工具
*   `std::next_permutation`：生成全排列（暴力搜索常用）。
*   `std::max_element` / `std::min_element`：找区间最值。
*   `std::reverse`：翻转序列。

---

## 6. 底层逻辑 vs STL 对比表

| 数据结构/算法   | 《算法导论》手写底层                 | C++ STL 工具            | 复杂度           |
| :-------------- | :----------------------------------- | :---------------------- | :--------------- |
| **动态数组**    | `int *a = malloc(...)`               | `std::vector`           | 随机访问 $O(1)$  |
| **双向链表**    | `struct Node { Node *next, *prev; }` | `std::list`             | 插入 $O(1)$      |
| **堆/优先队列** | `max_heapify` / `build_heap`         | `std::priority_queue`   | 插入 $O(\log n)$ |
| **平衡搜索树**  | **红黑树 (Red-Black Tree)**          | `std::map` / `std::set` | 查询 $O(\log n)$ |
| **散列表**      | `hash_function` / `chaining`         | `std::unordered_map`    | 查询 $O(1)$      |
| **快速排序**    | `partition` / `quick_sort`           | `std::sort`             | $O(n \log n)$    |
| **快速选择**    | `randomized_select`                  | `std::nth_element`      | $O(n)$           |
| **二分查找**    | `while (low <= high)...`             | `std::lower_bound`      | $O(\log n)$      |

---

## 💡 助教的自学建议

1.  **不要因为有了 STL 就忘记底层**：
    在洛谷刷题时，你会发现有些题（如平衡树、高级堆优化）需要你修改底层逻辑，那时候 STL 的固定功能就不够用了。**懂底层的人才知道什么时候该用什么容器。**
2.  **理解“迭代器”**：
    迭代器 `it` 就像一个指针。`*it` 拿到值，`it++` 移到下一个。
    大部分 STL 算法都是区间左闭右开的：`[v.begin(), v.end())`。
3.  **大一新生的优势**：
    你已经学过了 CLRS，当你看到 `std::map` 时，你脑子里反映的是“节点染色和旋转”；当你看到 `std::priority_queue` 时，你想到的是“节点下沉和上浮”。**这种“透视”能力是你以后成为大牛的根本。**

