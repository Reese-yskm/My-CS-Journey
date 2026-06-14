/*
# Problem 4 智慧城市

当直接构造最优解很难时，先尝试把问题转化成：
给定一个候选范围 / 候选答案，我能不能判断它是否合法。
如果这个判断具有单调性，就可以考虑二分或贪心扩展。

## 一、题目内容

给定一个长度为 n 的整数序列 a，以及两个整数 m 和 k。

对于一个连续子序列，也就是一段区间 [l, r]，系统会从这段数据中选择若干个数据对：

    每个数据只能被使用一次；
    最多选择 m 对；
    使得这些数据对的差的平方和最大。

如果区间长度不足 2m，那么能选多少对就选多少对。

这个最大的平方差和，称为该区间的“波动指数”。

要求：

    将整个序列 a 分割成若干段连续子序列，
    使得每一段的波动指数都不超过 k。

问最少需要分成多少段。


## 二、题目本质

这题表面是“分割数组”，但不是普通 DP。

一开始容易想：

    每个位置都可能切，也可能不切；
    我是不是要考虑所有分法？

这样会非常复杂。

更好的角度是：

    从左到右构造每一段；
    当前这一段能往右扩多远，就尽量往右扩多远；
    扩不动了，就在这里切开。

也就是说：

    不要先想“在哪里切”；
    而是想“当前左端点固定时，最远合法右端点在哪里”。


## 三、为什么可以贪心分段？

关键是单调性。

固定左端点 l。

如果区间 [l, r] 是合法的，也就是波动指数 <= k，
那么继续扩展到 [l, r + 1] 后，波动指数可能仍然合法，也可能超过 k。

但是如果 [l, r] 已经不合法了，那么 [l, r + 1]、[l, r + 2] 也不会重新合法。

原因是：

    区间变长后，可选择的数据更多；
    系统要求的是“最大平方差和”；
    可选数据更多时，最大值不会变小，只会不变或变大。

所以固定 l 时，合法右端点具有这样的结构：

    l, l+1, l+2, ..., far        合法
    far+1, far+2, ..., n-1       不合法

这就是单调性。

因此，从 l 开始，选最远的合法右端点 far 是合理的。

因为目标是段数最少，如果当前段能取到 far，
你却提前在 mid < far 处切开，那么前面少拿了一些元素，
后面剩下的元素更多，不可能让总段数变少。

所以贪心策略是：

    每一段都取当前能取到的最长合法段。


## 四、整体算法框架

用 l 表示当前段的左端点。

    ans = 0
    l = 0

    while l < n:
        找到从 l 开始的最远合法右端点 best
        ans++
        l = best + 1

最后 ans 就是最少段数。


## 五、如何判断一个区间是否合法？

假设要判断闭区间 [l, r]。

题目中的波动指数是：

    在区间中最多选 m 对数，
    每个数只能用一次，
    让所有差的平方和最大。

为了让平方差和最大，应该把最小的和最大的配对，第二小的和第二大的配对，依次类推。

例如区间数据排序后是：

    1 2 7 8 9

如果 m = 2，那么最优配对是：

    1 和 9
    2 和 8

平方差和为：

    (9 - 1)^2 + (8 - 2)^2

因此判断区间 [l, r] 的方法是：

    1. 取出 a[l..r]
    2. 排序
    3. 令 len = r - l + 1
    4. pairs = min(m, len / 2)
    5. 从两端向中间配对：
           最大 - 最小
           第二大 - 第二小
           ...
    6. 计算平方差和，看是否 <= k


## 六、为什么不是只看最大值和最小值？

如果 m = 1，确实只需要看：

    (最大值 - 最小值)^2

但是如果 m > 1，就不够了。

因为系统可以选择多对数据。

例如：

    1 2 7 8 9

如果 m = 2，那么不仅要考虑 1 和 9，
还要考虑 2 和 8。

所以一般情况要看：

    最大 - 最小
    第二大 - 第二小
    第三大 - 第三小
    ...

一共取 min(m, len / 2) 对。


## 七、为什么可以用二分找右端点？

固定左端点 l 时：

    [l, r] 合法性随着 r 增大具有单调性。

更具体地说：

    如果 [l, mid] 合法，那么可以尝试更长的区间；
    如果 [l, mid] 不合法，那么右端点太靠右了，需要缩短。

所以可以二分右端点 r。

对于每一个 l：

    left = l
    right = n - 1
    best = l

    while left <= right:
        mid = (left + right) / 2
        if ok(l, mid):
            best = mid
            left = mid + 1
        else:
            right = mid - 1

最后 best 就是从 l 开始的最远合法右端点。


## 八、这次暴露出来的问题

### 1. 能理解思想，但上手写判断函数容易乱

这题的区间判断本身不难：

    排序
    两端作差
    平方求和

但是实际写的时候容易出现这些错误：

    sort 的右端点是否包含？
    原数组下标和临时数组下标是否混了？
    len 怎么算？
    第 t 小和第 t 大分别在哪里？
    平方差是否会 int 溢出？

所以以后遇到这种“排序后两端配对”的问题，最好统一用临时数组写法。

即：

    vector<int> b;
    for i from l to r:
        b.push_back(a[i]);

    sort(b.begin(), b.end());

这样 b 的下标就是 0 到 len - 1，
不会再和原数组的 l、r 混在一起。


### 2. 二分不是单独技巧，而是建立在单调性上的

这题不是看到“找最远”就机械二分。

真正顺序应该是：

    先证明合法性有单调性；
    再知道固定左端点时合法右端点是一段连续范围；
    然后才可以用二分找最远合法位置。

二分的本质是利用：

    合法 合法 合法 不合法 不合法 不合法

这种结构。


### 3. 分段问题不一定都要 DP

一看到“最少分成多少段”，容易想到 DP。

但是这题有一个特殊性质：

    当前段越长越好，而且不会伤害后面的最优性。

所以可以贪心。

以后遇到类似问题，可以先问：

    固定左端点时，能不能找到最远合法右端点？
    当前段尽量长会不会影响最优性？
    合法性是否具有单调性？

如果答案都比较明确，就优先考虑贪心分段。


## 九、AC 代码：暴力判断 + 二分右端点

说明：

    这份代码重点是逻辑清晰，适合作为第一版。
    ok 函数每次都会复制区间并排序，所以不是极限优化版。
    但非常适合理解这题的核心框架。
*/

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/*
    判断闭区间 [l, r] 是否合法。

    参数：
        a      原数组
        l, r   当前判断的闭区间
        m      最多选择的数据对数量
        limit  波动指数阈值，也就是题目中的 k

    返回：
        true  表示 [l, r] 的波动指数 <= limit
        false 表示 [l, r] 的波动指数 > limit
*/
bool ok(const vector<int> &a, int l, int r, int m, ll limit)
{
    /*
        为了避免下标混乱，直接把区间 [l, r] 拷贝到临时数组 b 中。

        这样 b 的下标就是：
            0 到 len - 1

        后面两端配对时更清晰。
    */
    vector<int> b;

    for (int i = l; i <= r; i++)
    {
        b.push_back(a[i]);
    }

    /*
        排序后：
            b[0] 是最小值
            b[len - 1] 是最大值
            b[1] 是第二小
            b[len - 2] 是第二大
            ...
    */
    sort(b.begin(), b.end());

    int len = r - l + 1;

    /*
        区间长度为 len，最多只能组成 len / 2 对。
        题目最多允许选 m 对。

        所以实际选择的对数为：
            min(m, len / 2)
    */
    int pairs = min(m, len / 2);

    ll sum = 0;

    /*
        两端配对。

        第 t 对：
            左边：b[t]，表示第 t+1 小
            右边：b[len - 1 - t]，表示第 t+1 大

        t 从 0 开始。
    */
    for (int t = 0; t < pairs; t++)
    {
        ll diff = b[len - 1 - t] - b[t];

        sum += diff * diff;

        /*
            如果已经超过 limit，就可以提前返回 false。
            没必要继续算。
        */
        if (sum > limit)
        {
            return false;
        }
    }

    return sum <= limit;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
    {
        int n, m;
        ll k;

        cin >> n >> m >> k;

        vector<int> a(n);

        for (int i = 0; i < n; i++)
        {
            cin >> a[i];
        }

        /*
            ans 表示已经分出的段数。
            l 表示当前这一段的左端点。
        */
        int ans = 0;
        int l = 0;

        /*
            从左到右贪心分段。
        */
        while (l < n)
        {
            /*
                对于当前左端点 l，
                二分查找最远的合法右端点 best。
            */
            int left = l;
            int right = n - 1;
            int best = l;

            while (left <= right)
            {
                int mid = left + (right - left) / 2;

                /*
                    如果 [l, mid] 合法，
                    说明当前区间还能尝试扩得更长。
                */
                if (ok(a, l, mid, m, k))
                {
                    best = mid;
                    left = mid + 1;
                }
                /*
                    如果 [l, mid] 不合法，
                    说明右端点太靠右，要往左缩。
                */
                else
                {
                    right = mid - 1;
                }
            }

            /*
                [l, best] 就是当前能取到的最长合法段。
                把这一段切下来。
            */
            ans++;

            /*
                下一段从 best + 1 开始。
            */
            l = best + 1;
        }

        cout << ans << "\n";
    }

    return 0;
}

/*
## 十、用样例理解

样例：

    n = 5, m = 1, k = 49
    a = 8 2 1 7 9

因为 m = 1，所以一个区间的波动指数就是：

    (最大值 - 最小值)^2

如果看整个区间：

    最大值 = 9
    最小值 = 1

    (9 - 1)^2 = 64

64 > 49，所以整个区间不合法。

从左端点 0 开始尽量扩：

    [8]          合法
    [8,2]        (8-2)^2 = 36，合法
    [8,2,1]      (8-1)^2 = 49，合法
    [8,2,1,7]    (8-1)^2 = 49，合法
    [8,2,1,7,9]  (9-1)^2 = 64，不合法

所以第一段可以取：

    8 2 1 7

第二段是：

    9

答案为 2。


第二组：

    n = 5, m = 1, k = 64
    a = 8 2 1 7 9

整个区间波动指数为：

    (9 - 1)^2 = 64

刚好不超过 64，所以整段合法。

答案为 1。


## 十一、复杂度分析

设 n 为数组长度。

主循环最多产生若干段。
每一段通过二分查找右端点，二分次数为 O(log n)。

每次 ok(l, r)：
    需要复制区间，最坏 O(n)
    需要排序，最坏 O(n log n)
    需要两端配对，最坏 O(m)

所以这份朴素版本的复杂度偏高，大约可以理解为：

    O(段数 * log n * n log n)

它适合作为第一版理解和验证思路。

如果数据范围更大，就需要优化 ok 函数，
比如维护有序结构，或者使用更复杂的数据结构来动态维护区间中的若干极值。


## 十二、这题最重要的收获

### 1. 先找性质，再选算法

这题的核心不是马上写代码，而是先发现：

    固定左端点时，区间合法性具有单调性。

这个性质一旦确定，就能推出：

    可以找最远合法右端点；
    可以用二分；
    可以贪心分段。

这比直接想“所有分法”要清楚得多。


### 2. 分段题不一定是 DP

看到“最少分成多少段”，确实可以联想到 DP。

但是如果每一段有“越长越好”的性质，并且当前取最长不会让后面更差，
就应该考虑贪心。

这题就是：

    当前段能合法地多吃一个元素，就没有理由提前切。


### 3. 判断区间合法性时，避免下标混乱

如果直接在原数组上写：

    sort(b.begin() + l, b.begin() + r + 1)

也可以。

但对于初学阶段，更推荐：

    把 [l, r] 拷贝成一个新数组 b；
    对 b 整体排序；
    用 b[0] 和 b[len - 1] 配对。

这样代码更长一点，但不容易写错。


### 4. 两端配对是一种常见模型

当题目要求让差值尽可能大时，常见策略是：

    最大配最小；
    第二大配第二小；
    依次向中间靠拢。

这是一种很常见的排序贪心模型。


### 5. 二分要依赖单调性

二分不是随便加的优化。

本题能二分，是因为固定 l 后，右端点 r 的合法性形如：

    true true true false false false

所以可以二分最后一个 true。


### 6. 代码中的变量要有清晰含义

这题中建议统一使用：

    l, r       表示原数组区间端点
    len        表示区间长度
    pairs      表示实际选择的数据对数量
    limit      表示阈值 k
    t          表示第几对
    diff       表示一对数的差

变量含义清楚，下标就不容易乱。


## 十三、一句话总结

这题的关键转化是：

    先证明区间合法性随右端点单调；
    再从左到右贪心取最长合法段；
    每段是否合法通过排序后两端配对计算波动指数；
    固定左端点时，用二分找最远合法右端点。
*/
/*
## 十四、优化方向：不要每次完整排序整个区间

上一版 ok(l, r) 的写法是：

    1. 复制 a[l..r]
    2. 对整个区间排序
    3. 两端配对
    4. 计算平方差和

单次 ok 的复杂度大约是：

    O(len log len)

其中 len = r - l + 1。

但是仔细想一下，计算波动指数时，我们并不需要知道整个区间的完整排序结果。

我们只需要：

    最小的 pairs 个数
    最大的 pairs 个数

其中：

    pairs = min(m, len / 2)

因为波动指数只会用到：

    最小值 和 最大值
    第二小 和 第二大
    第三小 和 第三大
    ...

所以如果 m 相对较小，完整排序整个区间就有点浪费。

可以优化为：

    只找出 pairs 个最小值和 pairs 个最大值。

C++ 中可以用 nth_element。


## 十五、nth_element 是什么？

nth_element 的作用是：

    把第 n 小的元素放到它最终排序后应该在的位置上；
    它左边的元素都 <= 它；
    它右边的元素都 >= 它；
    但左右两边内部不保证有序。

例如：

    nth_element(b.begin(), b.begin() + p, b.end());

执行后：

    b[0] 到 b[p - 1] 里面就是 p 个较小的元素，
    但这 p 个元素内部不一定有序。

如果我们想要 p 个最小值，并且按照从小到大排序，可以：

    nth_element(...)
    取出前 p 个
    sort(这 p 个)

同理，如果想要 p 个最大值，可以：

    nth_element(b.begin(), b.end() - p, b.end());

执行后：

    b.end() - p 到 b.end() 这一段就是 p 个较大的元素。


## 十六、优化后的 ok 函数

核心思路：

    1. 复制区间 [l, r] 到 b
    2. 计算 pairs = min(m, len / 2)
    3. 用 nth_element 找出 pairs 个最小值
    4. 用 nth_element 找出 pairs 个最大值
    5. 分别排序这两部分
    6. 最小的和最大的配对，计算平方差和

这样单次 ok 的复杂度大约变成：

    O(len + pairs log pairs)

相比完整排序：

    O(len log len)

当 m 比较小的时候，这个优化会比较明显。


## 十七、优化版 ok 函数代码
*/

bool ok_fast(const vector<int> &a, int l, int r, int m, long long limit)
{
    vector<int> b;

    for (int i = l; i <= r; i++)
    {
        b.push_back(a[i]);
    }

    int len = r - l + 1;
    int pairs = min(m, len / 2);

    /*
        如果 pairs == 0，说明区间长度不足 2，
        无法组成任何一对，波动指数为 0。
    */
    if (pairs == 0)
    {
        return true;
    }

    /*
        取出 pairs 个最小值。
    */
    nth_element(b.begin(), b.begin() + pairs, b.end());

    vector<int> low;

    for (int i = 0; i < pairs; i++)
    {
        low.push_back(b[i]);
    }

    /*
        low 内部还没排序。
        为了按“最小、第二小、第三小...”配对，
        需要排序。
    */
    sort(low.begin(), low.end());

    /*
        取出 pairs 个最大值。

        注意：
            nth_element 可以在同一个 b 上再次使用。
            因为我们已经把 low 复制出来了。
    */
    nth_element(b.begin(), b.end() - pairs, b.end());

    vector<int> high;

    for (int i = len - pairs; i < len; i++)
    {
        high.push_back(b[i]);
    }

    /*
        high 内部也没排序。
        我们希望按照“最大、第二大、第三大...”使用，
        所以降序排序。
    */
    sort(high.begin(), high.end(), greater<int>());

    long long sum = 0;

    for (int t = 0; t < pairs; t++)
    {
        long long diff = 1LL * high[t] - low[t];

        sum += diff * diff;

        if (sum > limit)
        {
            return false;
        }
    }

    return true;
}

/*
## 十八、优化版完整代码

这份代码和上一份主体逻辑完全一样：

    从左到右贪心分段；
    固定左端点 l；
    二分找最远合法右端点；
    用 ok_fast 判断区间是否合法。

只是把原来的 ok 函数替换成了 ok_fast。
*/

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

bool ok_fast(const vector<int> &a, int l, int r, int m, long long limit)
{
    vector<int> b;

    for (int i = l; i <= r; i++)
    {
        b.push_back(a[i]);
    }

    int len = r - l + 1;
    int pairs = min(m, len / 2);

    if (pairs == 0)
    {
        return true;
    }

    /*
        找 pairs 个最小值。
    */
    nth_element(b.begin(), b.begin() + pairs, b.end());

    vector<int> low;

    for (int i = 0; i < pairs; i++)
    {
        low.push_back(b[i]);
    }

    sort(low.begin(), low.end());

    /*
        找 pairs 个最大值。
    */
    nth_element(b.begin(), b.end() - pairs, b.end());

    vector<int> high;

    for (int i = len - pairs; i < len; i++)
    {
        high.push_back(b[i]);
    }

    sort(high.begin(), high.end(), greater<int>());

    ll sum = 0;

    for (int t = 0; t < pairs; t++)
    {
        ll diff = 1LL * high[t] - low[t];

        sum += diff * diff;

        if (sum > limit)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
    {
        int n, m;
        ll k;

        cin >> n >> m >> k;

        vector<int> a(n);

        for (int i = 0; i < n; i++)
        {
            cin >> a[i];
        }

        int ans = 0;
        int l = 0;

        while (l < n)
        {
            int left = l;
            int right = n - 1;
            int best = l;

            while (left <= right)
            {
                int mid = left + (right - left) / 2;

                if (ok_fast(a, l, mid, m, k))
                {
                    best = mid;
                    left = mid + 1;
                }
                else
                {
                    right = mid - 1;
                }
            }

            ans++;
            l = best + 1;
        }

        cout << ans << "\n";
    }

    return 0;
}

/*
## 十九、这次优化的意义

### 1. 原版 ok 的问题

原版每次判断区间是否合法，都要对整个区间排序：

    sort(b.begin(), b.end());

这会得到完整的有序序列。

但事实上，波动指数只用到了两端的若干个元素：

    pairs 个最小值
    pairs 个最大值

中间的元素根本不会参与配对。

所以完整排序做了额外工作。


### 2. 优化版 ok 的核心

优化版只取：

    pairs 个最小值
    pairs 个最大值

然后只对这两小段排序。

因此复杂度从：

    O(len log len)

变成大约：

    O(len + pairs log pairs)

其中：

    len = 当前区间长度
    pairs = min(m, len / 2)


### 3. 什么时候优化明显？

如果 m 很小，而区间很长，优化明显。

例如：

    len = 10000
    m = 5

原版会排序 10000 个数。

优化版只需要找出：

    5 个最小值
    5 个最大值

然后排序这两个长度为 5 的数组。

这就明显快很多。


### 4. 什么时候优化不明显？

如果 m 接近 len / 2，那么 pairs 很大。

例如：

    len = 10000
    m = 5000

这时候几乎所有数都会参与配对，
优化版和完整排序的差距就没那么大。


### 5. nth_element 适合用来“只关心前 k 个/后 k 个”的问题

以后看到这些需求，可以想到 nth_element：

    找第 k 小
    找前 k 小
    找前 k 大
    不要求整体有序

如果要求整体有序，还是用 sort。

如果只关心一部分极值，可以优先考虑 nth_element。


## 二十、进一步优化的思考

当前优化版仍然有一个问题：

    每次 ok 都要重新复制区间。

因为主逻辑里面对每个左端点 l 做二分，
每次二分都会调用 ok_fast，
而 ok_fast 每次都会重新处理 [l, mid]。

所以它不是最终极限优化。

更高级的优化方向可能是：

    用数据结构动态维护当前区间中的元素；
    当右端点移动时，把新元素插入结构；
    快速查询当前区间的 pairs 个最小值和 pairs 个最大值。

可能用到：

    multiset
    树状数组
    线段树
    值域离散化
    维护前 k 小和前 k 大

但这已经比当前复习题的核心思想更复杂了。

对于这道题的复习价值来说，更重要的是掌握：

    单调性
    贪心分段
    二分右端点
    排序后两端配对
    nth_element 局部优化


## 二十一、优化版一句话总结

原版判断区间是否合法时，会完整排序整个区间；

优化版发现波动指数只需要两端的 pairs 个元素，
所以用 nth_element 只取 pairs 个最小值和 pairs 个最大值，
再分别排序后两端配对，从而减少不必要的排序开销。
*/