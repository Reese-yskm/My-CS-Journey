# Python 基础语法对照表（C++ 迁移版）

> 适合已经有 C++ 基础、刚开始写 Python 时查阅。  
> 核心目标：不是从零学编程，而是把已有的 C++ 思维迁移到 Python 写法。

---

## 0. 最重要的差异

| 方面 | C++ | Python |
|---|---|---|
| 代码块 | 用 `{}` | 用缩进 |
| 语句结尾 | 通常用 `;` | 一般不用 `;` |
| 变量类型 | 需要声明类型 | 通常不用声明类型 |
| 主函数 | `int main()` | 通常直接写脚本，也可写 `main()` |
| 注释 | `//` 或 `/* */` | `#`，多行字符串可临时当注释 |
| 布尔值 | `true / false` | `True / False` |
| 空值 | `nullptr` | `None` |
| 逻辑运算 | `&&`, `||`, `!` | `and`, `or`, `not` |
| 下标 | 从 0 开始 | 从 0 开始，且支持负下标 |

Python 最需要注意的是：

```python
if x > 0:
    print("positive")
else:
    print("non-positive")
```

缩进就是语法的一部分，不能乱缩进。

---

## 1. 输出

### 1.1 基本输出

C++：

```cpp
cout << x << endl;
```

Python：

```python
print(x)
```

---

### 1.2 输出多个变量

C++：

```cpp
cout << a << " " << b << endl;
```

Python：

```python
print(a, b)
```

默认用空格分隔，末尾换行。

---

### 1.3 自定义分隔符和结尾

```python
print(a, b, c, sep=",")   # 用逗号分隔
print(a, end=" ")          # 输出后不换行，改为空格
```

---

### 1.4 格式化输出

推荐用 f-string：

```python
name = "Tom"
age = 18
print(f"{name} is {age} years old")
```

保留小数：

```python
x = 3.1415926
print(f"{x:.2f}")   # 3.14
```

宽度控制：

```python
print(f"{x:10.2f}")
```

---

## 2. 输入

### 2.1 输入一个整数

C++：

```cpp
int x;
cin >> x;
```

Python：

```python
x = int(input())
```

---

### 2.2 输入一个浮点数

```python
x = float(input())
```

---

### 2.3 输入一个字符串

```python
s = input()
```

---

### 2.4 一行输入多个整数

C++：

```cpp
int a, b;
cin >> a >> b;
```

Python：

```python
a, b = map(int, input().split())
```

---

### 2.5 输入一组整数到列表

C++：

```cpp
vector<int> a(n);
for (int i = 0; i < n; i++) cin >> a[i];
```

Python：

```python
a = list(map(int, input().split()))
```

---

### 2.6 先输入 n，再输入 n 个数

```python
n = int(input())
a = list(map(int, input().split()))
```

注意：这里默认这 n 个数在同一行。

如果 n 个数分多行输入：

```python
n = int(input())
a = []
for _ in range(n):
    a.append(int(input()))
```

---

### 2.7 更快的输入

做算法题时，如果输入很多，可以用：

```python
import sys
input = sys.stdin.readline

n = int(input())
```

注意：`sys.stdin.readline()` 读入的字符串末尾可能带 `\n`，一般 `int()` 和 `split()` 不受影响。

---

## 3. 变量与基本类型

## 3.1 整数、浮点数、字符串、布尔值

C++：

```cpp
int a = 10;
double b = 3.14;
string s = "hello";
bool ok = true;
```

Python：

```python
a = 10
b = 3.14
s = "hello"
ok = True
```

---

## 3.2 查看类型

```python
print(type(a))
```

---

## 3.3 类型转换

```python
int("123")       # 字符串转整数
float("3.14")   # 字符串转浮点数
str(123)         # 数字转字符串
list("abc")      # ['a', 'b', 'c']
```

---

## 3.4 空值 None

C++ 里常见：

```cpp
nullptr
```

Python 里用：

```python
None
```

判断是否为空值：

```python
if x is None:
    print("empty")
```

---

## 4. 运算符

## 4.1 算术运算

| 运算 | C++ | Python | 说明 |
|---|---|---|---|
| 加 | `a + b` | `a + b` |  |
| 减 | `a - b` | `a - b` |  |
| 乘 | `a * b` | `a * b` |  |
| 除 | `a / b` | `a / b` | Python 中结果通常是浮点数 |
| 整除 | `a / b` | `a // b` | Python 整除用 `//` |
| 取模 | `a % b` | `a % b` |  |
| 幂 | `pow(a,b)` | `a ** b` |  |

例子：

```python
print(5 / 2)    # 2.5
print(5 // 2)   # 2
print(2 ** 10)  # 1024
```

---

## 4.2 比较运算

```python
x == y
x != y
x > y
x >= y
x < y
x <= y
```

Python 支持连续比较：

```python
if 0 <= x <= 100:
    print("valid")
```

相当于：

```cpp
if (0 <= x && x <= 100)
```

---

## 4.3 逻辑运算

C++：

```cpp
if (a > 0 && b > 0) {}
if (a > 0 || b > 0) {}
if (!ok) {}
```

Python：

```python
if a > 0 and b > 0:
    pass

if a > 0 or b > 0:
    pass

if not ok:
    pass
```

---

## 5. 条件语句

C++：

```cpp
if (x > 0) {
    cout << "positive";
} else if (x == 0) {
    cout << "zero";
} else {
    cout << "negative";
}
```

Python：

```python
if x > 0:
    print("positive")
elif x == 0:
    print("zero")
else:
    print("negative")
```

注意：Python 是 `elif`，不是 `else if`。

---

## 6. 循环

## 6.1 for 循环

C++：

```cpp
for (int i = 0; i < n; i++) {
    cout << i << endl;
}
```

Python：

```python
for i in range(n):
    print(i)
```

`range(n)` 产生：

```text
0, 1, 2, ..., n-1
```

---

## 6.2 指定起点和终点

```python
for i in range(1, n + 1):
    print(i)
```

产生：

```text
1, 2, ..., n
```

---

## 6.3 指定步长

```python
for i in range(0, n, 2):
    print(i)
```

产生：

```text
0, 2, 4, ...
```

---

## 6.4 倒序循环

C++：

```cpp
for (int i = n - 1; i >= 0; i--) {}
```

Python：

```python
for i in range(n - 1, -1, -1):
    print(i)
```

含义：从 `n-1` 到 `0`，每次减 1。

---

## 6.5 while 循环

C++：

```cpp
while (x > 0) {
    x--;
}
```

Python：

```python
while x > 0:
    x -= 1
```

---

## 6.6 break 和 continue

```python
for x in a:
    if x < 0:
        continue
    if x == 0:
        break
    print(x)
```

和 C++ 含义一样：

- `continue`：跳过本轮循环
- `break`：结束整个循环

---

## 7. 列表 list

Python 的 `list` 可以粗略理解为 C++ 的 `vector`。

## 7.1 创建列表

C++：

```cpp
vector<int> a;
vector<int> b(n, 0);
```

Python：

```python
a = []
b = [0] * n
```

---

## 7.2 常用操作

| 操作 | C++ vector | Python list |
|---|---|---|
| 添加元素 | `a.push_back(x)` | `a.append(x)` |
| 删除末尾 | `a.pop_back()` | `a.pop()` |
| 长度 | `a.size()` | `len(a)` |
| 访问第 i 个 | `a[i]` | `a[i]` |
| 清空 | `a.clear()` | `a.clear()` |
| 判空 | `a.empty()` | `len(a) == 0` 或 `not a` |

---

## 7.3 下标访问

```python
a = [10, 20, 30, 40]

print(a[0])    # 10
print(a[1])    # 20
print(a[-1])   # 40，最后一个
print(a[-2])   # 30，倒数第二个
```

---

## 7.4 遍历列表

只遍历元素：

```python
for x in a:
    print(x)
```

遍历下标：

```python
for i in range(len(a)):
    print(i, a[i])
```

同时遍历下标和元素：

```python
for i, x in enumerate(a):
    print(i, x)
```

---

## 7.5 切片

切片是 Python 里很常用的写法。

```python
a = [0, 1, 2, 3, 4, 5]

print(a[1:4])    # [1, 2, 3]
print(a[:3])     # [0, 1, 2]
print(a[3:])     # [3, 4, 5]
print(a[:])      # 复制整个列表
print(a[::-1])   # 反转列表
```

规律：

```python
a[l:r]
```

表示取下标 `[l, r)`，包含 `l`，不包含 `r`。

---

## 7.6 排序

原地排序：

```python
a.sort()
```

降序排序：

```python
a.sort(reverse=True)
```

返回排序后的新列表：

```python
b = sorted(a)
```

按规则排序：

```python
students = [("Tom", 80), ("Alice", 95), ("Bob", 90)]
students.sort(key=lambda x: x[1])
```

表示按分数排序。

---

## 7.7 列表推导式

C++ 里通常写循环：

```cpp
vector<int> b;
for (int x : a) {
    b.push_back(x * x);
}
```

Python 可以写：

```python
b = [x * x for x in a]
```

带条件：

```python
b = [x for x in a if x % 2 == 0]
```

表示筛出所有偶数。

---

## 8. 字符串 str

## 8.1 基本操作

```python
s = "hello"

print(len(s))     # 长度
print(s[0])       # 'h'
print(s[-1])      # 'o'
print(s[1:4])     # 'ell'
```

字符串也支持切片。

---

## 8.2 字符串不可修改

Python 字符串不能直接修改某个字符：

```python
s = "hello"
# s[0] = 'H'  # 错误
```

可以转成列表再改：

```python
chars = list(s)
chars[0] = 'H'
s = ''.join(chars)
```

---

## 8.3 split 和 join

拆分字符串：

```python
s = "I love Python"
words = s.split()
print(words)   # ['I', 'love', 'Python']
```

按逗号拆分：

```python
s = "a,b,c"
parts = s.split(",")
```

拼接字符串：

```python
words = ["I", "love", "Python"]
s = " ".join(words)
print(s)   # I love Python
```

---

## 8.4 常用字符串方法

```python
s.lower()          # 转小写
s.upper()          # 转大写
s.strip()          # 去掉首尾空白字符
s.replace("a", "b")
s.startswith("abc")
s.endswith("xyz")
s.find("abc")     # 找不到返回 -1
```

---

## 9. 元组 tuple

元组可以理解为“不可修改的列表”。

```python
t = (1, 2, 3)
print(t[0])
```

常用于存一组固定信息：

```python
student = ("Tom", 18, 90)
```

也常用于函数返回多个值：

```python
def get_pos():
    return 3, 5

x, y = get_pos()
```

---

## 10. 字典 dict

Python 的 `dict` 类似 C++ 的 `map` / `unordered_map`。

## 10.1 创建字典

C++：

```cpp
map<string, int> mp;
mp["apple"] = 3;
```

Python：

```python
mp = {}
mp["apple"] = 3
```

---

## 10.2 常用操作

```python
mp = {}
mp["apple"] = 3
mp["banana"] = 5

print(mp["apple"])
```

判断 key 是否存在：

```python
if "apple" in mp:
    print(mp["apple"])
```

删除 key：

```python
del mp["apple"]
```

---

## 10.3 get 方法

统计出现次数时很常用：

```python
cnt = {}
for x in a:
    cnt[x] = cnt.get(x, 0) + 1
```

`cnt.get(x, 0)` 的意思是：

- 如果 `x` 在字典中，返回 `cnt[x]`
- 如果不在，返回默认值 `0`

---

## 10.4 遍历字典

遍历 key：

```python
for key in mp:
    print(key)
```

遍历 key 和 value：

```python
for key, value in mp.items():
    print(key, value)
```

遍历 value：

```python
for value in mp.values():
    print(value)
```

---

## 11. 集合 set

Python 的 `set` 类似 C++ 的 `set` / `unordered_set`。

## 11.1 创建集合

```python
st = set()
st.add(3)
st.add(5)
```

注意：空集合不能写成 `{}`，因为 `{}` 是空字典。

---

## 11.2 常用操作

```python
st.add(x)       # 添加
st.remove(x)    # 删除，如果不存在会报错
st.discard(x)   # 删除，如果不存在不报错
len(st)         # 集合大小
```

判断是否存在：

```python
if x in st:
    print("yes")
```

---

## 11.3 去重

```python
a = [1, 2, 2, 3, 3, 3]
b = list(set(a))
```

注意：这样会去重，但不保证保持原顺序。

---

## 11.4 集合运算

```python
a = {1, 2, 3}
b = {3, 4, 5}

print(a | b)   # 并集 {1,2,3,4,5}
print(a & b)   # 交集 {3}
print(a - b)   # 差集 {1,2}
```

---

## 12. 栈和队列

## 12.1 栈 stack

Python 里直接用 list 当栈。

C++：

```cpp
stack<int> st;
st.push(1);
st.push(2);
st.pop();
```

Python：

```python
st = []
st.append(1)
st.append(2)
x = st.pop()
```

---

## 12.2 队列 queue

Python 推荐用 `deque`。

C++：

```cpp
queue<int> q;
q.push(1);
q.pop();
```

Python：

```python
from collections import deque

q = deque()
q.append(1)
q.append(2)
x = q.popleft()
```

---

## 13. 函数

## 13.1 定义函数

C++：

```cpp
int add(int a, int b) {
    return a + b;
}
```

Python：

```python
def add(a, b):
    return a + b
```

---

## 13.2 没有返回值

```python
def hello():
    print("hello")
```

没有显式 `return` 时，默认返回 `None`。

---

## 13.3 默认参数

```python
def greet(name="Tom"):
    print(f"Hello, {name}")

greet()
greet("Alice")
```

---

## 13.4 返回多个值

```python
def calc(a, b):
    return a + b, a * b

s, p = calc(3, 4)
```

本质上是返回了一个元组。

---

## 13.5 类型标注

Python 可以写类型标注，但它不强制检查类型。

```python
def add(a: int, b: int) -> int:
    return a + b
```

这对自己读代码很有帮助。

---

## 14. 常用内置函数

| 函数 | 作用 |
|---|---|
| `len(x)` | 求长度 |
| `sum(a)` | 求和 |
| `max(a)` | 最大值 |
| `min(a)` | 最小值 |
| `abs(x)` | 绝对值 |
| `round(x)` | 四舍五入 |
| `sorted(a)` | 返回排序后的新列表 |
| `range(n)` | 生成循环范围 |
| `enumerate(a)` | 同时获得下标和元素 |
| `zip(a, b)` | 并行遍历两个序列 |
| `map(func, seq)` | 对序列每个元素应用函数 |
| `filter(func, seq)` | 筛选元素 |

例子：

```python
a = [3, 1, 4, 1, 5]

print(len(a))
print(sum(a))
print(max(a))
print(min(a))
print(sorted(a))
```

---

## 15. 常用模块

## 15.1 math

```python
import math

print(math.sqrt(2))
print(math.gcd(12, 18))
print(math.factorial(5))
print(math.pi)
```

---

## 15.2 collections.Counter

统计频率：

```python
from collections import Counter

a = [1, 2, 2, 3, 3, 3]
cnt = Counter(a)

print(cnt[3])   # 3
```

相当于更方便的 `map<int, int>` 计数。

---

## 15.3 collections.defaultdict

```python
from collections import defaultdict

cnt = defaultdict(int)
for x in a:
    cnt[x] += 1
```

普通字典中，如果 key 不存在，直接 `cnt[x] += 1` 会报错。  
`defaultdict(int)` 会自动把不存在的 key 初始为 `0`。

---

## 15.4 heapq 优先队列

Python 的 `heapq` 默认是小根堆。

C++：

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
```

Python：

```python
import heapq

pq = []
heapq.heappush(pq, 3)
heapq.heappush(pq, 1)
heapq.heappush(pq, 2)

x = heapq.heappop(pq)   # 1
```

如果要大根堆，通常存相反数：

```python
heapq.heappush(pq, -x)
max_x = -heapq.heappop(pq)
```

---

## 16. 文件读写

## 16.1 读取整个文件

```python
with open("input.txt", "r", encoding="utf-8") as f:
    content = f.read()
```

---

## 16.2 按行读取

```python
with open("input.txt", "r", encoding="utf-8") as f:
    for line in f:
        line = line.strip()
        print(line)
```

---

## 16.3 写入文件

```python
with open("output.txt", "w", encoding="utf-8") as f:
    f.write("hello\n")
    f.write("world\n")
```

---

## 16.4 追加写入

```python
with open("output.txt", "a", encoding="utf-8") as f:
    f.write("new line\n")
```

---

## 17. 异常处理

类似 C++ 的 `try-catch`，Python 用 `try-except`。

```python
try:
    x = int(input())
    print(10 / x)
except ValueError:
    print("输入的不是整数")
except ZeroDivisionError:
    print("不能除以 0")
```

一般初学阶段不用大量写异常处理，但读文件、转类型时会用到。

---

## 18. 类 class

初学 Python 时可以先不深入面向对象，但至少知道基本写法。

C++：

```cpp
struct Student {
    string name;
    int score;
};
```

Python：

```python
class Student:
    def __init__(self, name, score):
        self.name = name
        self.score = score

stu = Student("Tom", 90)
print(stu.name, stu.score)
```

`self` 可以理解为 C++ 里的 `this`，但 Python 中必须显式写出来。

---

## 19. Python 的 main 写法

小脚本可以直接写：

```python
n = int(input())
print(n)
```

稍微正式一点，推荐写成：

```python
def main():
    n = int(input())
    print(n)

if __name__ == "__main__":
    main()
```

可以先记住这个模板，不必一开始深究。

---

## 20. 常见算法写法对照

## 20.1 求和、最大值、最小值

C++：

```cpp
int sum = 0;
for (int x : a) sum += x;
```

Python：

```python
total = sum(a)
mx = max(a)
mn = min(a)
```

---

## 20.2 统计出现次数

C++：

```cpp
map<int, int> cnt;
for (int x : a) cnt[x]++;
```

Python 普通写法：

```python
cnt = {}
for x in a:
    cnt[x] = cnt.get(x, 0) + 1
```

Python 推荐写法：

```python
from collections import Counter
cnt = Counter(a)
```

---

## 20.3 判断元素是否存在

C++：

```cpp
if (st.count(x)) {}
```

Python：

```python
if x in st:
    pass
```

对列表、集合、字典都可以用 `in`，但集合和字典查询更快。

---

## 20.4 二维数组

C++：

```cpp
vector<vector<int>> a(n, vector<int>(m, 0));
```

Python：

```python
a = [[0] * m for _ in range(n)]
```

不要写成：

```python
a = [[0] * m] * n
```

这个写法会导致每一行其实引用同一个列表，容易出大 bug。

---

## 20.5 DFS 模板

```python
def dfs(u):
    visited[u] = True
    for v in graph[u]:
        if not visited[v]:
            dfs(v)
```

---

## 20.6 BFS 模板

```python
from collections import deque

def bfs(start):
    q = deque()
    q.append(start)
    visited[start] = True

    while q:
        u = q.popleft()
        for v in graph[u]:
            if not visited[v]:
                visited[v] = True
                q.append(v)
```

---

## 21. 常见坑

## 21.1 缩进错误

Python 靠缩进区分代码块：

```python
if x > 0:
    print("positive")
```

不能随便多空格或少空格。

---

## 21.2 `=` 和 `==`

```python
x = 3      # 赋值
x == 3     # 判断是否相等
```

---

## 21.3 整除要用 `//`

```python
print(5 / 2)    # 2.5
print(5 // 2)   # 2
```

算法题里如果想要整数除法，通常用 `//`。

---

## 21.4 空列表不要当默认参数

不推荐：

```python
def f(a=[]):
    a.append(1)
    return a
```

推荐：

```python
def f(a=None):
    if a is None:
        a = []
    a.append(1)
    return a
```

---

## 21.5 二维数组初始化坑

错误写法：

```python
a = [[0] * m] * n
```

正确写法：

```python
a = [[0] * m for _ in range(n)]
```

---

## 21.6 Python 变量只是“名字”

```python
a = [1, 2, 3]
b = a
b.append(4)
print(a)   # [1, 2, 3, 4]
```

如果想复制列表：

```python
b = a[:]
# 或
b = a.copy()
```

---

## 22. 一个完整小例子

任务：输入一组整数，统计每个数字出现次数。

输入：

```text
6
1 2 2 3 3 3
```

Python：

```python
from collections import Counter

def main():
    n = int(input())
    a = list(map(int, input().split()))

    cnt = Counter(a)

    for key, value in cnt.items():
        print(key, value)

if __name__ == "__main__":
    main()
```

---

## 23. 学习顺序建议

如果你有 C++ 基础，建议按这个顺序熟悉 Python：

1. 输入输出、变量、if、for、while
2. list、str、dict、set
3. 函数、模块导入
4. 文件读写
5. collections、heapq、math
6. 用 Python 写几个小脚本
7. 再学类、包管理、虚拟环境、第三方库

一句话总结：

> 你不是重新学编程，而是把 C++ 已有能力迁移到 Python 的表达方式上。 
