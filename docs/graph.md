### 图论模版

模版中的 `INF` 均代表无限大。

建议在 c++ 文件前声明：

```cpp
#define INF 0x3f3f3f3f
```



#### 最短路

-   注意：下标均从 1 开始

##### Dijkstra (朴素版)

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
##### Dijkstra (堆优化)

-   时间复杂度 O(ElogE)
-   适用于稀疏图

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

bool operator<(const node &a, const node &b)
{
    return a.dis > b.dis;
}

const int MAXN = 10010;

int dis[MAXN];
vector<node> G[MAXN];
priority_queue<node> q;

void dijkstra(int s)
{
    memset(dis, 0x3f, sizeof(dis));

    dis[s] = 0;
    q.push(node(s, 0));

    while (!q.empty())
    {
        node tmp = q.top(); q.pop();
        int len = G[tmp.to].size();
        for (int i = 0; i < len; i++)
        {
            int to = G[tmp.to][i].to;
            int d = G[tmp.to][i].dis;
            if (dis[to] > tmp.dis + d)
            {
                dis[to] = tmp.dis + d;
                q.push(node(to, dis[to]));
            }
        }
    }
}
```

##### SPFA (SLF 优化) (带路径)

-   时间复杂度 O(VE)
-   可以处理负权值
-   返回 `true` 时代表存在最短路，返回 `false` 时代表存在负环。
-   时间不稳定，没有负权值建议使用 Dijkstra 算法

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

vector<node> G[MAXN];
bool in[MAXN];
int cnt[MAXN], dis[MAXN], path[MAXN];

bool SPFA(int s)
{
    deque<int> q;
    int x, to;

    memset(cnt, 0, sizeof(cnt));
    memset(in, 0, sizeof(in));
    memset(dis, 0x3f, sizeof(dis));
    memset(path, -1, sizeof(-1));

    q.push_back(s);
    cnt[s]++;
    dis[s] = 0;
    in[s] = true;

    while (!q.empty())
    {
        x = q.front();
        q.pop_front();
        in[x] = false;
        int len = G[x].size();
        for (int i = 0; i < len; i++)
        {
            to = G[x][i].to;
            if ((dis[x] < INF) && (dis[to] > dis[x] + G[x][i].dis))
            {
                dis[to] = dis[x] + G[x][i].dis;
                path[to] = x;
                if (!in[to])
                {
                    in[to] = true;
                    cnt[to]++;
                    if (cnt[to] == n)
                        return false;
                    if (!q.empty())
                    {
                        if (dis[to] > dis[q.front()])
                            q.push_back(to);
                        else
                            q.push_front(to);
                    }
                    else
                        q.push_back(to);
                }
            }
        }
    }
    return true;
}

void print_path(int x)
{
    if (x == -1)
        return;
    print_path(path[x]);
    cout << x << " ";
}
```