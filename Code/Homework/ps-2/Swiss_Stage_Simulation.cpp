/**
 * @file Swiss_Stage_Simulation.cpp
 * @brief 瑞士轮赛制模拟 - 分组、Bu 分、首尾配对、严格调用伪随机函数
 *
 * 【题目描述】
 * 有 2^n 支队伍参加瑞士轮比赛。每支队伍初始有一个种子编号：
 * 编号越小，表示这支队伍越强。
 *
 * 每场比赛：
 *  - 胜者胜场 +1
 *  - 负者负场 +1
 *
 * 当一支队伍：
 *  - 胜场达到 k ：晋级，离开瑞士轮阶段
 *  - 负场达到 k ：淘汰，离开瑞士轮阶段
 *
 * 每一轮比赛前，需要将所有“还没离开的队伍”按 (win, lose) 分组。
 * 同一组内：
 *  - 所有队伍的胜负场完全相同
 *  - 先比较 Bu 分，Bu 分更高的更强
 *  - 若 Bu 分相同，则种子编号更小的更强
 *  - 每次取最强的一支与最弱的一支比赛，直到这一组全部配对完成
 *
 * 不同组之间：
 *  - 必须先处理“胜场更多”的组
 *  - 例如 2-1 组要先于 1-2 组完成比赛
 *
 * 【Bu 分定义】
 * 一支队伍的 Bu 分，等于它此前所有对手“当前”的 (win - lose) 之和。
 *
 * 注意：
 * 1. Bu 分在一整轮内固定。必须在这一轮开始时统一计算好。
 * 2. 如果之前多次遇到同一个对手，则每次都要重复累计。
 * 3. 如果还没打过任何比赛，则 Bu 分为 0。
 *
 * 【比赛结果如何决定】
 * 题目给出一个伪随机函数：
 *
 * static unsigned long long seed = 1;
 * unsigned int my_rand(void) {
 *     seed = seed * 1103515245 + 12345;
 *     return seed & 1;
 * }
 *
 * 每场比赛必须严格调用一次该函数：
 *  - 返回 1 ：强队赢
 *  - 返回 0 ：弱队赢
 *
 * 【输出要求】
 * 对每一个查询队伍，按比赛顺序输出：
 *  - 它遇到过的所有对手名字
 *  - 最后输出它的最终战绩 win-lose
 *
 * 例如：
 * 对手1 对手2 对手3 ... win-lose
 *
 * --------------------------------------------------
 * 【核心思路】
 *
 * 这是一道“复杂规则模拟题”，关键不是高深算法，而是把规则拆成稳定模块：
 *
 * 1. 为每支队伍维护完整状态：
 *    - 队名
 *    - 种子编号
 *    - 当前 Bu 分
 *    - 当前胜场 / 负场
 *    - 是否已经离开瑞士轮
 *    - 历史对手列表（用下标记录）
 *    - 每场结果（本题输出不直接用到，但保留下来便于完整记录）
 *
 * 2. 每一轮开始时：
 *    - 先统一计算所有在赛队伍的 Bu 分
 *    - 再按 (win, lose) 分组
 *    - 再按“胜场多的组先处理”
 *    - 每组内部按“强到弱”排序
 *    - 然后首尾配对：最强打最弱，次强打次弱……
 *
 * 3. 每一场比赛：
 *    - 题目规定返回 1 表示强队获胜，返回 0 表示弱队获胜
 *    - 因此 play_match() 的参数约定为：
 *          第一个参数一定是强队，第二个参数一定是弱队
 *    - 这样函数内部就不用再次判断强弱
 *
 * 4. 整个瑞士轮阶段：
 *    - 不断执行“单轮模拟”
 *    - 直到所有队伍都达到 k 胜或 k 负，全部离开瑞士轮
 *
 * --------------------------------------------------
 * 【为什么要存对手下标，而不是直接存 Team 对象？】
 *
 * 因为队伍状态（win / lose / bu）会随着比赛不断变化。
 * 如果把整个 Team 对象拷贝进历史记录，那么后续计算 Bu 分时，
 * 取到的会是“旧拷贝”，而不是对手当前战绩。
 *
 * 所以：
 *  - 历史对手只存“对手下标”
 *  - 需要当前状态时，统一去总队伍数组 teams 中查询
 *
 * --------------------------------------------------
 * 【时间复杂度分析（大致）】
 *
 * 设总队伍数为 m = 2^n。
 * 每轮：
 *  - 计算 Bu：遍历当前在赛队伍及其历史对手
 *  - 分组：O(m)
 *  - 各组排序：总的排序代价在本数据范围内可接受
 *
 * 数据范围 n <= 14，所以 m <= 16384。
 * 本题重点是正确模拟，而不是极限优化。
 */

#include <bits/stdc++.h>
using namespace std;

// --------------------------------------------------
// 题目规定的伪随机函数：不能改动，必须严格按比赛顺序调用
// --------------------------------------------------
static unsigned long long seed = 1;
unsigned int my_rand(void)
{
    seed = seed * 1103515245 + 12345;
    return seed & 1;
}

// --------------------------------------------------
// 队伍结构体
// --------------------------------------------------
struct Team
{
    string name;           // 队伍名称
    int seed_id = 0;       // 种子编号，越小越强
    int bu = 0;            // 当前轮固定使用的 Bu 分
    int win = 0;           // 当前胜场
    int lose = 0;          // 当前负场
    bool isleave = false;  // 是否已经晋级或淘汰

    vector<int> opponents; // 历史对手下标（按比赛发生顺序记录）
    vector<int> result;    // 每场结果：1 表示自己赢，0 表示自己输
};

// --------------------------------------------------
// 计算某支队伍当前的 Bu 分
//
// 定义：此前所有对手当前的 (win - lose) 之和
// 如果之前多次遇到同一个对手，则要重复累计
// --------------------------------------------------
int compute_bu(const vector<Team> &teams, int id)
{
    int sum = 0;
    for (int opp : teams[id].opponents)
    {
        sum += teams[opp].win - teams[opp].lose;
    }
    return sum;
}

// --------------------------------------------------
// 进行一场比赛
//
// 约定：传入时 strong 一定比 weak 强
// 规则：my_rand() == 1 => 强队赢；否则弱队赢
//
// 这里要更新：
// 1. 双方历史对手
// 2. 双方胜负场
// 3. 双方每场比赛结果
// 4. 是否达到 k 胜 / k 负而离开瑞士轮
// --------------------------------------------------
void play_match(vector<Team> &teams, int strong, int weak, int k)
{
    // 双方记录彼此为历史对手
    teams[strong].opponents.push_back(weak);
    teams[weak].opponents.push_back(strong);

    unsigned int x = my_rand();

    if (x == 1)
    {
        // 强队获胜
        teams[strong].win++;
        teams[weak].lose++;
        teams[strong].result.push_back(1);
        teams[weak].result.push_back(0);
    }
    else
    {
        // 弱队获胜
        teams[weak].win++;
        teams[strong].lose++;
        teams[strong].result.push_back(0);
        teams[weak].result.push_back(1);
    }

    // 达到 k 胜或 k 负，则离开瑞士轮
    if (teams[strong].win == k || teams[strong].lose == k)
        teams[strong].isleave = true;
    if (teams[weak].win == k || teams[weak].lose == k)
        teams[weak].isleave = true;
}

// --------------------------------------------------
// 模拟一整轮比赛
//
// 一轮中的关键步骤：
// 1. 统一计算所有在赛队伍的 Bu 分（这一轮内固定）
// 2. 按 (win, lose) 分组
// 3. 按“胜场更多的组优先”处理
// 4. 每组内部按“强到弱”排序
// 5. 首尾配对并比赛
//
// 注意：
// - Bu 分不能边打一边更新，必须先统一算完
// - 每组内部排序后，直接首尾配对，不要打一场后重排
// --------------------------------------------------
void play_one_round(vector<Team> &teams, int k)
{
    // ---------- 1. 统一计算本轮 Bu 分 ----------
    for (int i = 0; i < (int)teams.size(); i++)
    {
        if (!teams[i].isleave)
        {
            teams[i].bu = compute_bu(teams, i);
        }
    }

    // ---------- 2. 按 (win, lose) 分组 ----------
    map<pair<int, int>, vector<int>> groups;
    for (int i = 0; i < (int)teams.size(); i++)
    {
        if (!teams[i].isleave)
        {
            groups[{teams[i].win, teams[i].lose}].push_back(i);
        }
    }

    // ---------- 3. 决定各组的处理顺序 ----------
    // 题目要求：胜场更多的组先处理
    vector<pair<int, int>> order;
    for (auto &it : groups)
    {
        order.push_back(it.first);
    }

    sort(order.begin(), order.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         {
             if (a.first != b.first)
                 return a.first > b.first; // 胜场高的先
             return a.second < b.second;   // 若需要，再让负场少的在前
         });

    // ---------- 4. 逐组处理 ----------
    for (auto key : order)
    {
        vector<int> vec = groups[key];

        // 组内按“强到弱”排序：
        // 先比较 Bu 分（高者强），Bu 相同则种子编号小者强
        sort(vec.begin(), vec.end(), [&](int a, int b)
             {
                 if (teams[a].bu != teams[b].bu)
                     return teams[a].bu > teams[b].bu;
                 return teams[a].seed_id < teams[b].seed_id;
             });

        // ---------- 5. 首尾配对 ----------
        // 最强 vs 最弱，次强 vs 次弱……
        int m = (int)vec.size();
        for (int j = 0; j < m / 2; j++)
        {
            int strong = vec[j];
            int weak = vec[m - 1 - j];
            play_match(teams, strong, weak, k);
        }
    }
}

// --------------------------------------------------
// 判断是否还有队伍仍在瑞士轮中
// --------------------------------------------------
bool has_active_team(const vector<Team> &teams)
{
    for (const auto &t : teams)
    {
        if (!t.isleave)
            return true;
    }
    return false;
}

// --------------------------------------------------
// 输出某支队伍的比赛历史
//
// 题目真正要求输出的是：
// - 这支队伍按顺序遇到过的所有对手
// - 最终战绩 win-lose
//
// 不输出：
// - 自己名字
// - 每场比赛的赢家
// --------------------------------------------------
void print_team(const vector<Team> &teams, int id)
{
    for (int i = 0; i < (int)teams[id].opponents.size(); i++)
    {
        if (i)
            cout << ' ';
        cout << teams[teams[id].opponents[i]].name;
    }
    if (!teams[id].opponents.empty())
        cout << ' ';
    cout << teams[id].win << '-' << teams[id].lose << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --------------------------------------------------
    // 输入：
    // n, k, q
    // 共有 2^n 支队伍
    // 接着读入 2^n 个队伍名字，按输入顺序给予种子编号 1..2^n
    // 再读入 q 个查询队名
    // --------------------------------------------------
    int n, k, q;
    cin >> n >> k >> q;

    int m = 1 << n; // 队伍总数
    vector<Team> teams(m);
    unordered_map<string, int> id_of; // 队名 -> 下标
    id_of.reserve(m * 2);

    for (int i = 0; i < m; i++)
    {
        cin >> teams[i].name;
        teams[i].seed_id = i + 1; // 输入顺序就是种子编号
        id_of[teams[i].name] = i;
    }

    vector<string> queries(q);
    for (int i = 0; i < q; i++)
    {
        cin >> queries[i];
    }

    // --------------------------------------------------
    // 主循环：不断进行瑞士轮，直到所有队伍都离开
    // --------------------------------------------------
    while (has_active_team(teams))
    {
        play_one_round(teams, k);
    }

    // --------------------------------------------------
    // 按查询顺序输出结果
    // --------------------------------------------------
    for (int i = 0; i < q; i++)
    {
        print_team(teams, id_of[queries[i]]);
    }

    return 0;
}

/**
 * 【本题最关键的实现注意事项】
 *
 * 1. Bu 分是一轮一算，不是打一场变一次
 *    正确做法：
 *      - 一轮开始时统一计算所有在赛队伍的 Bu
 *      - 这一轮中分组、排序、比赛都使用这个固定值
 *
 * 2. 不能把 Team 对象直接存进历史对手列表
 *    否则以后 win / lose 变化后，历史里存的是旧拷贝
 *    正确做法：
 *      - 只存对手下标
 *      - 要看当前状态时，回到总数组 teams 中查询
 *
 * 3. play_match() 中不要重复比较强弱
 *    因为外层已经先排序，再首尾配对：
 *      - vec[0] 一定最强
 *      - vec.back() 一定最弱
 *    所以只要保证参数传入顺序不乱即可
 *
 * 4. my_rand() 一场比赛只能调用一次
 *    而且必须严格按照题目规定的比赛发生顺序调用
 *    一旦调用顺序错了，后面的所有结果都会连锁出错
 *
 * 5. 输出格式容易看错
 *    题目只要求输出：
 *      - 按顺序遇到过的所有对手
 *      - 最终战绩
 *    不需要输出自己名字，也不需要输出每场比赛的赢家
 *
 * 【本题收获】
 *
 * 1. 复杂模拟题要先拆模块，不要一上来整题硬写
 *    这题就很适合拆成：
 *      - Team 结构体
 *      - compute_bu
 *      - play_match
 *      - play_one_round
 *      - 主循环与输出
 *
 * 2. 状态变化题里，尽量用“编号/下标”统一管理对象
 *    比直接复制对象更稳，也更容易避免状态不同步
 *
 * 3. 规则题最怕“差不多对”
 *    特别是这种带伪随机函数的题，顺序只要错一点，答案就全错
 *    所以必须逐条把题意落到代码里
 */ 
