### 数据结构模版



#### 并查集 (DSU)

-   `init()` ：初始化整个并查集
-   `find(int x)` ：返回该集合的根结点
-   `merge(int a, int b)` ：连接两个集合
-   `connected(int a, int b)` ：查找两个元素是否属于一个集合

```cpp
const int MAXN = 1010;

class dsu
{
private:
    int nd[MAXN];
public:
    dsu()
    {
        init();
    }
    void init()
    {
        for (int i = 0; i < MAXN; i++)
            nd[i] = i;
    }
    int find(int x)
    {
        return nd[x] == x ? x : nd[x] = find(nd[x]);
    }
    void merge(int a, int b)
    {
        nd[find(a)] = find(b);
    }
    bool connected(int a, int b)
    {
        return find(a) == find(b);
    }
};
```



#### 主席树 (可持久化线段树)

-   样例为: 查询区间 `[l, r]` 内的第 `k` 小值

```cpp
const int MAXN = 200010;

int node_cnt, n, m;
int sum[MAXN << 5], rt[MAXN], lc[MAXN << 5], rc[MAXN << 5];
int a[MAXN], b[MAXN];

void build(int &t, int l, int r)
{
    t = ++node_cnt;
    if (l == r)
        return;
    int mid = (l + r) >> 1;
    build(lc[t], l, mid);
    build(rc[t], mid + 1, r);
}

int modify(int t, int l, int r, int p)
{
    int rt = ++node_cnt;
    lc[rt] = lc[t];
    rc[rt] = rc[t];
    sum[rt] = sum[t] + 1;
    if (l == r)
        return rt;
    int mid = (l + r) >> 1;
    if (p <= mid)
        lc[rt] = modify(lc[rt], l, mid, p);
    else
        rc[rt] = modify(rc[rt], mid + 1, r, p);
    return rt;
}

int query(int u, int v, int l, int r, int k)
{
    int ans, mid = ((l + r) >> 1), x = sum[lc[v]] - sum[lc[u]];
    if (l == r)
        return l;
    if (x >= k)
        ans = query(lc[u], lc[v], l, mid, k);
    else
        ans = query(rc[u], rc[v], mid + 1, r, k - x);
    return ans;
}

int main()
{
    int l, r, k, q, ans;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i += 1)
        scanf("%d", &a[i]), b[i] = a[i];
    sort(b + 1, b + n + 1);
    q = unique(b + 1, b + n + 1) - b - 1;
    build(rt[0], 1, q);
    for (int i = 1; i <= n; i += 1)
    {
        int p = lower_bound(b + 1, b + q + 1, a[i]) - b;
        rt[i] = modify(rt[i - 1], 1, q, p);
    }
    while (m--)
    {
        scanf("%d%d%d", &l, &r, &k);
        ans = query(rt[l - 1], rt[r], 1, q, k);
        printf("%d\n", b[ans]);
    }
    return 0;
}
```