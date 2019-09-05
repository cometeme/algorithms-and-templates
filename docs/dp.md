# 动态规划模版

## 树形 dp

### 求树的直径长度

-   `res` 即为树的直径

```cpp
const int MAXN = 200010, MAXM = 200010 << 1;
 
int h[MAXN], tot = 0;
int f1[MAXN], f2[MAXN];
 
struct edge
{
    int v, nxt, w;
    edge() { v = nxt = w = 0; }
    edge(int v, int nxt, int w) { this->v = v, this->nxt = nxt, this->w = w; }
} e[MAXM];
 
void add_edge(int x, int y, int w)
{
    tot++;
    e[tot].v = y;
    e[tot].nxt = h[x];
    e[tot].w = w;
    h[x] = tot;
}
 
int res = 0;
 
void dp(int x, int pre)
{
    for (int i = h[x]; i; i = e[i].nxt)
    {
        int v = e[i].v;
        int w = e[i].w;

        if (v == pre)
            continue;

        dp(v, x);

        if (f1[x] < f1[v] + w)
            f2[x] = f1[x], f1[x] = f1[v] + w;
        else if (f2[x] < f1[v] + w)
            f2[x] = f1[v] + w;

        res = max(res, f1[x] + f2[x]);
    }
}
```



## 状压 DP 优化 (SOS DP)

-   时间复杂度 $O(n2^n)$

```cpp
const int N = 22;
int a[1 << N], dp[1 << N]

for (int i = 0; i < (1 << N); i++)
	dp[i] = a[i];

for (int i = 0; i < N; i++)
    for (int msk = 0; msk < (1 << N); msk++)
        if (msk & (1 << i))
            dp[msk] += dp[msk ^ (1 << i)];
```