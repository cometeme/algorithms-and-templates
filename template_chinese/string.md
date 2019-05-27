### 字符串模版

##### 拓展 kmp 算法 (exkmp)

-   求 `t` 与 `s` 的每一个后缀的最长公共前缀
-   `ext[i]` 表示 `t` 与 `s[i, n - 1]` 的最长公共前缀

```cpp
const int MAXN = 100010;

int nxt[MAXN], ext[MAXN];
string s, t;

void getnxt()
{
    nxt[0] = t.size();
    int cur = 0;
    while (t[cur] == t[1 + cur] && cur + 1 < (int)t.size())
        cur++;

    nxt[1] = cur;
    int p = 1;
    for (int i = 2; i < (int)t.size(); i++)
    {
        if (i + nxt[i - p] < nxt[p] + p)
            nxt[i] = nxt[i - p];
        else
        {
            int cur = nxt[p] + p - i;
            cur = max(cur, 0);
            while (t[cur] == t[i + cur] && i + cur < (int)t.size())
                cur++;
            nxt[i] = cur;
            p = i;
        }
    }
}

void exkmp()
{
    getnxt();
    int cur = 0;
    while (s[cur] == t[cur] && cur < min((int)s.size(), (int)t.size()))
        cur++;
    ext[0] = cur;
    int p = 0;
    for (int i = 1; i < (int)s.size(); i++)
    {
        if (i + nxt[i - p] < ext[p] + p)
            ext[i] = nxt[i - p];
        else
        {
            int cur = ext[p] + p - i;
            cur = max(cur, 0);
            while (t[cur] == s[i + cur] && cur < (int)t.size() && cur + i < (int)s.size())
                cur++;
            ext[i] = cur;
            p = i;
        }
    }
}
```



##### 字符串 Hash

-   运行前需要先调用 `init_hash()` 打表
-   当前版本适用于只包含 `a-z` 的字符串，如果包含其他字符可以手动修改
-   字符串 Hash 也适用于数值序列
-   因为 Hash 有概率会出错，采用双质数会好一些，可以采用 `p1 = 1e9 + 7` , `p2 = 1e9 + 9`

```cpp
const int MAXN = 100010;
const ull p = 1000000007;

ull base[MAXN];

void init_hash()
{
    base[0] = 1;
    for (int i = 1; i < MAXN; i++)
        base[i] = base[i - 1] * p;
}

void string_hash(const char s[], ull h[])
{
    h[0] = 0;
    int n = strlen(s);
    for (int i = 1; i <= n; i++)
        h[i] = h[i - 1] * p + (s[i - 1] - 'a');
}

ull get_hash(int l, int r, ull h[])
{
    return h[r] - h[l - 1] * base[r - l + 1];
}
```