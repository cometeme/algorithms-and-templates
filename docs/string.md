# 字符串模版

## 马拉车算法 (manacher)

-   返回最长回文子串的长度
-   注意 `ns` 和 `p` 数组的大小需要为原来字符串长度的两倍

```cpp
const int MAXN = 1000010;

char s[MAXN], ns[MAXN << 1];
int p[MAXN << 1];

int manacher(char s[], int len)
{
	for (int i = 0; i < len; i++)
	{
		ns[2 * i] = '#';
		ns[2 * i + 1] = s[i];
	}

	ns[len * 2] = '#';

	int mx = 0, id;

	for (int i = 1; i <= 2 * len; i++)
	{
		if (mx > i)
		    p[i] = min(p[2 * id - i], mx - i);

		else
		    p[i] = 1;

        while (ns[i - p[i]] == ns[i + p[i]] && (i - p[i] >= 0) && (i + p[i] <= len * 2))
            p[i]++;

        if (mx < i + p[i])
        {
            id = i;
            mx = i + p[i];
        }
	}

	int res = 0;
    
	for (int i = 1; i <= 2 * len; i++)
		res = max(res, p[i]);
	
	return res - 1;
}
```



## kmp

- `next[i]` 表示子串 `s[1:i]` 最长的相同前缀和后缀的长度

```cpp
const int MAXN = 1000010, MAXM = 1000010;

char s1[MAXN], s2[MAXM];
int nxt[MAXM];

void getnxt(char s[])
{
    nxt[0] = -1;
    int x = -1, len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        while (x != -1 && s[i] != s[x])
            x = nxt[x];
        nxt[i + 1] = ++x;
    }
}

int kmp(char a[], char b[])
{
    int cnt = 0, x = 0;
    int lena = strlen(a), lenb = strlen(b);

    for (int i = 0; i < lena; i++)
    {
        while (x != -1 && a[i] != b[x])
            x = nxt[x];
        x++;
        if (x == lenb)
        {
            cnt++;
            x = nxt[x];
        }
    }
    return cnt;
}
```



## 拓展 kmp 算法 (exkmp)

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



## 字符串 Hash

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