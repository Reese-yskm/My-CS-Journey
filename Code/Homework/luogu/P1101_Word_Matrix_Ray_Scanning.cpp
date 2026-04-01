/**
 * @file: P1101_Word_Matrix_Ray_Scanning.cpp
 * @brief: 洛谷 P1101 单词方阵 - 射线探测法 (Ray Scanning)
 * @author: [Your Name]
 * @date: 2026-04-01
 *
 *[题目背景]
 * 给定一个 N*N 的字母方阵，找出隐藏在其中的目标单词 "yizhong"。
 * 单词可以沿着 8 个方向连续摆放（不可拐弯），且单词之间可能交叉共用字母。
 * 要求将不是目标单词的字母替换为 '*' 并输出。
 *
 *[核心算法：从 DFS 到射线探测]
 * 1. 为什么不用 DFS？
 *    DFS 适用于“每走一步都需要重新决策方向”的迷宫探索。
 *    本题单词长度固定 (7)，方向一旦确定就必须笔直延伸（不可拐弯）。
 *
 * 2. 射线探测模型 (Ray Scanning)：
 *    利用解析几何中的射线方程：P = P_start + m * dir
 *    - 起点 P_start：(i, j)，即遍历到的首字母 'y'。
 *    - 方向向量 dir：(dx[k], dy[k])，遍历 8 个方向。
 *    - 延伸步数 m：从 0 到 6，对应单词长度。
 *    这种“向量倍乘”的技巧，将路径搜索降维成了直线扫描，三层循环即可搞定，逻辑极致清爽。
 *
 *[避坑与工程技巧]
 * 1. 状态解耦：单词可能交叉，直接修改原矩阵会破坏后续扫描。
 *    解决方案：开辟一个布尔标记矩阵 `res`，扫完所有可能后再统一渲染输出。
 * 2. 边界防御：越界检查必须是逻辑或 `||`，只要触碰任意一条边界即告失败。
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 1. 定义 8 个方向的向量（方向数组）
// 上下左右 + 四个对角线
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
string target = "yizhong";

int main()
{
    // 优化标准 IO 性能
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n))
        return 0;

    // alphas 存储原始矩阵，res 作为标记矩阵 (初始化为 false)
    vector<vector<char>> alphas(n, vector<char>(n));
    vector<vector<bool>> res(n, vector<bool>(n, false));

    // 读入方阵
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> alphas[i][j];
        }
    }

    // 2. 全局扫描：遍历每一个点作为可能的射线起点
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {

            // 如果命中首字母 'y'，开始向 8 个方向发射“射线”
            if (alphas[i][j] == 'y')
            {
                for (int k = 0; k < 8; k++)
                {
                    bool match = true;

                    // 3. 核心：沿着选定的方向向量进行倍乘延伸 (m 是步长)
                    for (int m = 0; m < 7; m++)
                    {
                        int nx = i + m * dx[k];
                        int ny = j + m * dy[k];

                        // 边界检查：必须使用逻辑或 ||
                        if (nx < 0 || nx >= n || ny < 0 || ny >= n)
                        {
                            match = false;
                            break;
                        }
                        // 字符匹配检查
                        if (alphas[nx][ny] != target[m])
                        {
                            match = false;
                            break;
                        }
                    }

                    // 4. 如果 7 步探测全部成功，将整条射线上的格子标记为 true
                    if (match)
                    {
                        for (int m = 0; m < 7; m++)
                        {
                            res[i + m * dx[k]][j + m * dy[k]] = true;
                        }
                    }
                }
            }
        }
    }

    // 5. 渲染分离：根据 res 标记矩阵输出最终结果
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (!res[i][j])
            {
                cout << '*';
            }
            else
            {
                cout << alphas[i][j];
            }
        }
        cout << "\n";
    }

    return 0;
}

/**
 * [深度复盘与收获]
 * 1. 视角的升维：
 *    方向数组不仅仅能做 `+1` 的相邻格行走（迷宫DFS模式），
 *    还能结合乘法 `m * dx` 做线性延伸。这让我深刻体会到了“坐标向量化”的强大威力。
 *
 * 2. 算法选型直觉：
 *    - 如果路径是曲折的、需要动态决策的 -> 使用 DFS / 回溯。
 *    - 如果路径是刚性的、长度与方向固定的 -> 放弃递归，直接使用“多重循环 + 射线探测”。
 *
 * 3. 业务逻辑解耦：
 *    先“无损探测并打标签（bool矩阵）”，最后再“集中渲染（修改或打印）”。
 *    这极大地简化了状态冲突（如单词交叉重叠）带来的麻烦，也是优秀工程代码的标配思维。
 */