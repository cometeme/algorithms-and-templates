# 数据结构模版



## 并查集 (DSU)

-   `init()` ：初始化整个并查集
-   `find(int x)` ：返回该集合的根结点
-   `merge(int a, int b)` ：连接两个集合
-   `connected(int a, int b)` ：查找两个元素是否属于一个集合

```cpp
const int MAXN = 100010;

struct dsu
{
    int nd[MAXN];
    void init(int n)
    {
        for (int i = 0; i <= n; i++)
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



## 树状数组

```cpp
const int MAXN = 500010;

struct BIT
{
    inline int lowbit(int x) { return x & (-x); }
    int n;
    ll t[MAXN];
    void init(int n)
    {
        this->n = n;
        memset(t + 1, 0, sizeof(t[0]) * n);
    }
    void add(int pos, int v)
    {
        for (; pos <= n; pos += lowbit(pos))
            t[pos] += v;
    }
    ll query(int pos)
    {
        ll res = 0;
        for (; pos > 0; pos -= lowbit(pos))
            res += t[pos];
        return res;
    }
} bit;
```



## 线段树

```cpp
const int MAXN = 100010;

ll a[MAXN], sum[MAXN << 2], lazyadd[MAXN << 2];

#define ls(x) (x << 1)
#define rs(x) (x << 1 | 1)

void pushup(int x)
{
    sum[x] = sum[ls(x)] + sum[rs(x)];
}
void pushdown(int x, int l, int r)
{
    if (!lazyadd[x])
        return;
    if (l == r)
    {
        lazyadd[x] = 0;
        return;
    }
    int mid = (l + r) >> 1;
    lazyadd[ls(x)] += lazyadd[x];
    lazyadd[rs(x)] += lazyadd[x];
    sum[ls(x)] += lazyadd[x] * (mid - l + 1);
    sum[rs(x)] += lazyadd[x] * (r - mid);
    lazyadd[x] = 0;
}
void build(int x, int l, int r)
{
    lazyadd[x] = 0;
    if (l == r)
    {
        sum[x] = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(ls(x), l, mid);
    build(rs(x), mid + 1, r);
    pushup(x);
}
void add(int x, int l, int r, int tl, int tr, int v)
{
    if (tl <= l && r <= tr)
    {
        sum[x] += v * (r - l + 1);
        lazyadd[x] += v;
        return;
    }
    pushdown(x, l, r);
    int mid = (l + r) >> 1;
    if (tl <= mid)
        add(ls(x), l, mid, tl, tr, v);
    if (tr > mid)
        add(rs(x), mid + 1, r, tl, tr, v);
    pushup(x);
}
void change(int x, int l, int r, int pos, int v)
{
    pushdown(x, l, r);
    if (l == r)
    {
        sum[x] = v;
        return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid)
        change(ls(x), l, mid, pos, v);
    if (pos > mid)
        change(rs(x), mid + 1, r, pos, v);
    pushup(x);
}
ll query(int x, int l, int r, int tl, int tr)
{
    if (tl <= l && r <= tr)
        return sum[x];
    pushdown(x, l, r);
    int mid = (l + r) >> 1;
    ll res = 0;
    if (tl <= mid)
        res += query(ls(x), l, mid, tl, tr);
    if (tr > mid)
        res += query(rs(x), mid + 1, r, tl, tr);
    return res;
}
```



## 主席树 (可持久化线段树)

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