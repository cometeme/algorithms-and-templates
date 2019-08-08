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
const int MAXN = 200010, MAXM = 200010;

int h[MAXN], tot = 0;

struct edge
{
    int u, v;
    ll w;
    edge() { u = v = w = 0; }
    edge(int u, int v, ll w) { this->u = u, this->v = v, this->w = w; }
} e[MAXM];

void add_edge(int x, int y, ll w)
{
    tot++;
    e[tot].u = y;
    e[tot].v = h[x];
    e[tot].w = w;
    h[x] = tot;
}

ll dis[MAXN];
bool vis[MAXN];

struct node
{
    int v;
    ll w;
    node() {v = w = 0;}
    node(int v, ll w) {this->v = v, this->w = w;}
};

bool operator<(const node &x, const node &y)
{
    return x.w > y.w;
}

priority_queue<node> p;

void dijkstra(int s)
{
    dis[s] = 0;

    p.push(node(s, 0));

    while (!p.empty())
    {
        node cur = p.top(); p.pop();

        if (vis[cur.v])
            continue;
        vis[cur.v] = true;

        int nxt = h[cur.v];

        while (nxt != -1)
        {
            if (dis[cur.v] + e[nxt].w < dis[e[nxt].u])
            {
                dis[e[nxt].u] = dis[cur.v] + e[nxt].w;
                p.push(node(e[nxt].u, dis[e[nxt].u]));
            }
            nxt = e[nxt].v;
        }
    }
}
```

### SPFA (SLF 优化)

-   时间复杂度 O(VE)
-   可以处理负权值
-   返回 `true` 时代表存在最短路，返回 `false` 时代表存在负环。
-   时间不稳定，没有负权值建议使用 Dijkstra 算法

```cpp
const int MAXN = 510, MAXM = 10010;

int h[MAXN], tot = 0, cnt[MAXN];
ll dis[MAXN];
bool vis[MAXN];

struct edge
{
    int u, v, w;
    edge() { u = v = w = 0; }
    edge(int u, int v, int w) { this->u = u, this->v = v, this->w = w; }
} e[MAXM];

void add_edge(int x, int y, ll w)
{
    tot++;
    e[tot].u = y;
    e[tot].v = h[x];
    e[tot].w = w;
    h[x] = tot;
}

bool SPFA(int s)
{
    deque<int> q;
    q.push_back(s);
    dis[s] = 0;
    vis[s] = true;
    cnt[s]++;

    while (!q.empty())
    {
        int cur = q.front(); q.pop_front();
        vis[cur] = false;

        int nxt = h[cur];
        while (nxt != -1)
        {
            if (dis[e[nxt].u] > dis[cur] + e[nxt].w)
            {
                dis[e[nxt].u] = dis[cur] + e[nxt].w;

                if (!vis[e[nxt].u])
                {
                    vis[e[nxt].u] = true;
                    cnt[e[nxt].u]++;

                    if (cnt[e[nxt].u] == n)
                        return false;

                    if (q.empty() || dis[e[nxt].u]>dis[q.front()])
                        q.push_back(e[nxt].u);
                    else
                        q.push_front(e[nxt].u);
                }
            }

            nxt = e[nxt].v;
        }
    }

    return true;
}
```