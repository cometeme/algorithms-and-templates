# 图论模版

模版中的 `INF` 均代表无限大。

建议在 c++ 文件前声明：

```cpp
#define INF 0x3f3f3f3f
```



## 最短路

-   注意：下标均从 1 开始

### Dijkstra (朴素版)

-   时间复杂度 O(V^2)
-   适用于稠密图

```cpp
struct node
{
    int to, dis;
    node(int p, int d)
    {
        to = p;
        dis = d;
    }
};

const int MAXN = 10010;
int n;

int dis[MAXN];
bool vis[MAXN];
vector<node> G[MAXN];

void dijkstra(int s)
{
    memset(dis, 0x3f, sizeof(dis));

    dis[s] = 0;
    int cnt = 0;

    while (cnt < n)
    {
        int m = INF;
        int x;
        for (int i = 1; i <= n; i++)
            if (!vis[i] && dis[i] < m)
            {
                m = dis[i];
                x = i;
            }

        cnt++;
        vis[x] = true;

        int len = G[x].size();
        for (int i = 0; i < len; i++)
        {
            int to = G[x][i].to;
            int d = G[x][i].dis;
            dis[to] = min(dis[to], dis[x] + d);
        }
    }
}
```
### Dijkstra (堆优化)

-   时间复杂度 O(ElogE)
-   适用于稀疏图

```cpp
const int MAXN = 100010, MAXM = 200010;

int h[MAXN], tot = 0;

struct edge
{
    int v, nxt;
    ll w;
    edge() { v = nxt = w = 0; }
    edge(int v, int nxt, ll w) { this->v = v, this->nxt = nxt, this->w = w; }
} e[MAXM];

void add_edge(int x, int y, ll w)
{
    tot++;
    e[tot].v = y;
    e[tot].nxt = h[x];
    e[tot].w = w;
    h[x] = tot;
}

ll dis[MAXN];
bool vis[MAXN];

struct node
{
    int v;
    ll w;
    node() { v = w = 0; }
    node(int v, ll w) { this->v = v, this->w = w; }
};

bool operator<(const node &x, const node &y)
{
    return x.w > y.w;
}

priority_queue<node> p;

void dijkstra(int s)
{
    memset(vis, 0, sizeof(vis));
    memset(dis, 0x3f, sizeof(dis));

    dis[s] = 0;

    p.push(node(s, 0));

    while (!p.empty())
    {
        node cur = p.top(); p.pop();

        if (vis[cur.v])
            continue;
        vis[cur.v] = true;

        for (int i = h[cur.v]; i; i = e[i].nxt)
            if (dis[cur.v] + e[i].w < dis[e[i].v])
            {
                dis[e[i].v] = dis[cur.v] + e[i].w;
                p.push(node(e[i].v, dis[e[i].v]));
            }
    }
}
```

### SPFA (SLF 优化)

-   时间复杂度 O(VE)
-   可以处理负权值
-   需要设置 `maxcnt` ，一般为顶点数 `n`
-   返回 `true` 时代表存在最短路，返回 `false` 时代表存在负环。
-   时间不稳定，没有负权值建议使用 Dijkstra 算法

```cpp
const int MAXN = 10010, MAXM = 500010;

int h[MAXN], tot = 0, cnt[MAXN];
ll dis[MAXN];
bool vis[MAXN];

struct edge
{
    int v, nxt;
    ll w;
    edge() { v = nxt = w = 0; }
    edge(int v, int nxt, ll w) { this->v = v, this->nxt = nxt, this->w = w; }
} e[MAXM];

void add_edge(int x, int y, ll w)
{
    tot++;
    e[tot].v = y;
    e[tot].nxt = h[x];
    e[tot].w = w;
    h[x] = tot;
}

bool SPFA(int s, int maxcnt)
{
    memset(vis, 0, sizeof(vis));
    memset(cnt, 0, sizeof(cnt));
    memset(dis, 0x3f, sizeof(dis));

    deque<int> q;
    q.push_back(s);
    dis[s] = 0;
    vis[s] = true;
    cnt[s]++;

    while (!q.empty())
    {
        int cur = q.front(); q.pop_front();
        vis[cur] = false;

        for (int i = h[cur]; i; i = e[i].nxt)
        {
            if (dis[e[i].v] > dis[cur] + e[i].w)
            {
                dis[e[i].v] = dis[cur] + e[i].w;

                if (!vis[e[i].v])
                {
                    vis[e[i].v] = true;
                    cnt[e[i].v]++;

                    if (cnt[e[i].v] >= maxcnt)
                        return false;

                    if (q.empty() || dis[e[i].v] > dis[q.front()])
                        q.push_back(e[i].v);
                    else
                        q.push_front(e[i].v);
                }
            }
        }
    }

    return true;
}
```




## 强连通分量 (Tarjan)

### 有向图

```cpp
const int MAXN = 200010, MAXM = MAXN << 1;
int h[MAXN], tot = 0;

struct edge
{
    int v, nxt;
    edge() { v = nxt = 0; }
    edge(int v, int nxt) { this->v = v, this->nxt = nxt; }
} e[MAXM];

void add_edge(int x, int y)
{
    tot++;
    e[tot].v = y;
    e[tot].nxt = h[x];
    h[x] = tot;
}

int s[MAXN], top;
int dfs_num, col_num;
int dfn[MAXN], low[MAXN], color[MAXN], colorcnt[MAXN];
bool vis[MAXN];

void Tarjan(int x)
{
    dfn[x] = ++dfs_num;
    low[x] = dfs_num;
    vis[x] = true;
    s[++top] = x;
    for (int i = h[x]; i; i = e[i].nxt)
    {
        int temp = e[i].v;
        if (!dfn[temp])
        {
            Tarjan(temp);
            low[x] = min(low[x], low[temp]);
        }
        else if (vis[temp])
            low[x] = min(low[x], dfn[temp]);
    }
    if (dfn[x] == low[x])
    {
        vis[x] = false;
        color[x] = ++col_num;
        colorcnt[col_num]++;
        while (s[top] != x)
        {
            color[s[top]] = col_num;
            colorcnt[col_num]++;
            vis[s[top--]] = false;
        }
        top--;
    }
}

void solve()
{
    memclr(dfn);
    memclr(low);
    memclr(color);
    memclr(colorcnt);
    memclr(vis);
    top = dfs_num = col_num = 0;

    for (int i = 1; i <= n; i++)
        if (dfn[i] == 0)
            Tarjan(i);
}
```

### 无向图

-   `cut` 非零代表该点为割点，必须要初始化 `root` 变量

```cpp
const int MAXN = 1010, MAXM = 1000 * 1000 + 10;

int h[MAXN], tot = 0;

struct edge
{
    int v, nxt;
    edge() { v = nxt = 0; }
    edge(int v, int nxt) { this->v = v, this->nxt = nxt; }
} e[MAXM];

void add_edge(int x, int y)
{
    tot++;
    e[tot].v = y;
    e[tot].nxt = h[x];
    h[x] = tot;
}

int dfs_num, root;
int dfn[MAXN], low[MAXN], cut[MAXN];
bool vis[MAXN];

void Tarjan(int u, int pre)
{
    dfn[u] = ++dfs_num;
    low[u] = dfs_num;
    vis[u] = true;
    int cnt = 0;
    for (int i = h[u]; i; i = e[i].nxt)
    {
        int v = e[i].v;

        if (v == pre)
            continue;
        
        if (!dfn[v])
        {
            Tarjan(v, u);
            cnt++;
            low[u] = min(low[u], low[v]);

            if (u == root && cnt > 1)
                cut[u]++;
            else if (u != root && low[v] >= dfn[u])
                cut[u]++;
        }
        else if (vis[v])
            low[u] = min(low[u], dfn[v]);
    }
}

void solve(int u)
{
    memclr(dfn);
    memclr(low);
    memclr(vis);
    memclr(cut);
    dfs_num = 0;
    root = u;

    Tarjan(u, -1);
}
```



## 匈牙利算法 (二分图最大匹配)

-   输入为 `x` 点的数量 `nx` ，输出最大匹配的个数

```cpp
const int MAXN = 1010 MAXM = MAXN;

int top = 0, s[MAXN];
int h[MAXN], tot = 0;
int mx[MAXN], my[MAXN];
bool vis[MAXN];

struct edge
{
    int v, nxt;
    edge() { v = nxt = 0; }
    edge(int v, int nxt) { this->v = v, this->nxt = nxt; }
} e[MAXM];

void add_edge(int x, int y)
{
    tot++;
    e[tot].v = y;
    e[tot].nxt = h[x];
    h[x] = tot;
}

bool dfs(int u)
{
    for (int i = h[u]; i; i = e[i].nxt)
    {
        int v = e[i].v;
        if (!vis[v])
        {
            vis[v] = 1, s[++top] = v;
            if (my[v] == -1 || dfs(my[v]))
            {
                my[v] = u;
                return 1;
            }
        }
    }
    return 0;
}

int hungary(int n)
{
    int res = 0;
    memset(my, -1, sizeof(my));
    for (int i = 1; i <= n; i++)
    {
        while (top)
            vis[s[top]] = 0, top--;
        res += dfs(i);
    }
    return res;
}
```



## KM 算法

-   需要指定 `x` 和 `y` 的点数 `cnt`
-   `lk[i]` 是与 `y[i]` 匹配的 `x` 的下标

```cpp
const int MAXN = 110;
int cnt;
bool visx[MAXN], visy[MAXN];
int lx[MAXN], ly[MAXN], lk[MAXN], w[MAXN][MAXN], slack[MAXN];

bool dfs(int x)
{
    visx[x] = true;
    for (int y = 1; y <= cnt; y++)
    {
        if (visy[y])
            continue;
        int t = lx[x] + ly[y] - w[x][y];
        if (t == 0)
        {
            visy[y] = true;
            if (lk[y] == -1 || dfs(lk[y]))
            {
                lk[y] = x;
                return true;
            }
        }
        else if (slack[y] > t)
            slack[y] = t;
    }
    return false;
}

void KM()
{
    memset(lk, -1, sizeof(lk));
    memset(lx, 0, sizeof(lx));
    memset(ly, 0, sizeof(ly));
    for (int i = 1; i <= cnt; i++)
        for (int j = 1; j <= cnt; j++)
            if (w[i][j] > lx[i])
                lx[i] = w[i][j];
    for (int x = 1; x <= cnt; x++)
    {
        for (int i = 1; i <= cnt; i++)
            slack[i] = INF;
        while (true)
        {
            memset(visx, 0, sizeof(visx));
            memset(visy, 0, sizeof(visy));
            if (dfs(x))
                break;
            int d = INF;
            for (int i = 1; i <= cnt; i++)
                if (!visy[i] && d > slack[i])
                    d = slack[i];
            for (int i = 1; i <= cnt; i++)
                if (visx[i])
                    lx[i] -= d;
            for (int i = 1; i <= cnt; i++)
            {
                if (visy[i])
                    ly[i] += d;
                else
                    slack[i] -= d;
            }
        }
    }
}
```