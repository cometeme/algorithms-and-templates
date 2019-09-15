# 数学/数论模版

需要注意：部分函数可能需要用到 `cmath` 等头文件内的函数。

模版中的 `ll` 全部代表 `long long` 。

建议在 c++ 文件前声明：

```cpp
#define ll long long
// 或者
typedef long long ll
```

## 快速乘

### 玄学版

```cpp
ll fastmul(ll a, ll b, ll mod)
{
    return a * b - (LL)((long double)a / mod * b + EPS) * mod;
}
```

### 普通版

```cpp
ll fastmul(ll a, ll b, ll m)
{
    ll res = 0, tmp = a % m;
    while (b > 0)
    {
        if (b & 1 == 1)
        {
            res += tmp;
            if (res > m)
                res -= m;
        }
        tmp <<= 1;
        if (tmp > m)
            tmp -= m;
        b >>= 1;
    }
    return res;
}
```



## 快速幂

### 不带模
```cpp
ll fastpow(ll base, ll exp)
{
    ll t, y;
    t = 1LL;
    y = base;
    while (exp != 0LL)
    {
        if ((exp & 1LL) == 1LL)
            t = t * y;
        y = y * y;
        exp = exp >> 1LL;
    }
    return t;
}
```

### 取模

```cpp
ll fastpow(ll base, ll exp, ll mod)
{
    ll t, y;
    t = 1LL;
    y = base;
    while (exp != 0LL)
    {
        if ((exp & 1LL) == 1LL)
            t = t * y % mod;
        y = y * y % mod;
        exp = exp >> 1LL;
    }
    return t % mod;
}
```

### 使用快速乘

```cpp
ll fastpow(ll a, ll b, ll m)
{
    ll res = 1, tmp = a;
    while (b > 0)
    {
        if (b & 1 == 1)
            res = fastmul(res, tmp, m);
        tmp = fastmul(tmp, tmp, m);
        b >>= 1;
    }
    return res;
}
```



## GCD (最大公约数)

### 欧几里得算法 (递归版)

```cpp
ll gcd(ll a, ll b)
{
    return (b == 0LL) ? a : gcd(b, a % b);
}
```

### 欧几里得算法 (迭代版)

```cpp
ll gcd(ll a, ll b)
{
    while (b != 0LL)
    {
        ll r = b;
        b = a % b;
        a = r;
    }
    return a;
}
```

### Stein 算法

```cpp
ll gcd(ll a, ll b)
{
    if (a == 0LL)
        return b;
    if (b == 0LL)
        return a;
    if (a % 2LL == 0LL && b % 2LL == 0LL)
        return 2LL * gcd(a >> 1LL, b >> 1LL);
    else if (a % 2LL == 0LL)
        return gcd(a >> 1LL, b);
    else if (b % 2LL == 0LL)
        return gcd(a, b >> 1LL);
    else
        return gcd(abs(a - b), min(a, b));
}
```



## exgcd

```cpp
ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if (a == 0LL && b == 0LL)
        return -1LL;
    if (b == 0LL)
    {
        x = 1LL, y = 0LL;
        return a;
    }
    ll d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
```



## LCM (最小公倍数)

-   需要使用最大公约数 `gcd` 函数。
-   注意可能会溢出

```cpp
ll lcm(ll a, ll b)
{
    return a / gcd(a, b) * b;
}
```



## 判断素数

### 6 优化方法

-   原理：除了 `2` 与 `3` 之外，质数一定出现在 `6` 的两侧。

```cpp
bool is_prime(ll x)
{
    if (x == 1LL)
        return false;
    if (x == 2LL || x == 3LL)
        return true;
    if (x % 6LL != 1LL && x % 6LL != 5LL)
        return false;

    for (ll i = 5LL; i * i <= x; i += 6LL)
        if (x % i == 0LL || x % (i + 2LL) == 0LL)
            return false;

    return true;
}
```

### Miller-Rabin

-   需要使用快速幂 `fastpow` 函数。
-   是一个 $O(1)$ 的算法
-   在 $10^{18}$ 以内取前 $12$ 个质数即可。

```cpp
bool test(ll n, ll k)
{
    if (fastpow(k, n - 1, n) != 1)
        return false;
    ll t = n - 1, tmp;
    while (t % 2 == 0)
    {
        t >>= 1;
        tmp = fastpow(k, t, n);
        if (tmp != 1 && tmp != n - 1)
            return false;
        if (tmp == n - 1)
            return true;
    }
    return true;
}
bool is_prime(ll n)
{
    if (n == 1 || (n > 2 && n % 2 == 0))
        return false;
    ll samples[14] = {12,
        2, 3, 5, 7,  // n < 3.2e9
        11, 13, 17, 19, 23, 29, 31, 37,  // n < 1.8e19
        41,  // n < 3.3e25
    };
    for (ll i = 1; i <= samples[0]; i++)
    {
        if (n == samples[i])
            return true;
        if (n > samples[i] && !test(n, samples[i]))
            return false;
    }
    return true;  // Certain prime
}
```



## 求逆元

### 费马小定理

-   需要使用快速幂 `fastpow` 函数。
-   注意： `p` 一定要是质数

```cpp
ll fermat(ll a, ll p)
{
    return fastpow(a, p - 2, p);
}
```

### exgcd (拓展欧几里得法)

* 注意： `p` 和 `a` 一定要互质

```cpp
ll mod_reverse(ll a, ll n)
{
    ll x, y, d = exgcd(a, n, x, y);
    if (d == 1)
    {
        if (x % n <= 0)
            return x % n + n;
        else
            return x % n;
    }
    else
        return -1LL;
}
```



## 组合数逆元

```cpp
const int mod = 998244353;
const int MAXN = 1000000;

ll fastpow(ll base, ll exp)
{
    ll t, y;
    t = 1LL;
    y = base;
    while (exp != 0LL)
    {
        if ((exp & 1LL) == 1LL)
            t = t * y % mod;
        y = y * y % mod;
        exp = exp >> 1LL;
    }
    return t % mod;
}

ll inv[MAXN + 10], f[MAXN + 10];
void init()
{
    f[0] = 1;
    for (int i = 1; i <= MAXN; i++)
        f[i] = f[i - 1] * i % mod;
    inv[MAXN] = fastpow(f[MAXN], mod - 2);
    for (int i = MAXN - 1; i >= 0; i--)
        inv[i] = inv[i + 1] * (i + 1) % mod;
}

ll C(ll n, ll m)
{
    if (n == m || m == 0)
        return 1;
    return f[n] * inv[m] % mod * inv[n - m] % mod;
}
```



## 质因子分解

### 普通版本

```cpp
vector<ll> factorize(ll n)
{
    vector<ll> fac;

    for (ll i = 2LL; i * i <= n; i++)
        if (n % i == 0)
        {
            fac.push_back(i);
            while (n % i == 0)
                n /= i;
        }

    if (n > 1)
        fac.push_back(n);

    return fac;
}
```



## 负数取模

* 将负数取模转化为正数结果

* -2 % 5 -> 3

```cpp
#define mod(a, b) (((a) % (b) + (b)) % (b))
```



## 最低 `1` 位置

* 返回 `x` 的二进制中最后一个 `1` 的位置。

* 下标从 1 开始，从右往左标记。

```cpp
#define lowbit(x) ((x) & (-x))
```



## 质数表

### 线性筛

-   时间复杂度：O(n)

```cpp
const int MAXN = 1000010;
int prime[MAXN], primesize = 0;
bool isprime[MAXN];
void get_prime()
{
    memset(isprime, true, sizeof(isprime));
    isprime[1] = false;
    for (int i = 2; i <= MAXN; i++)
    {
        if (isprime[i])
            prime[++primesize] = i;
        for (int j = 1; j <= primesize && i * prime[j] <= MAXN; j++)
        {
            isprime[i * prime[j]] = false;
            if (i % prime[j] == 0)
                break;
        }
    }
}
```

### 欧拉函数筛

```cpp
const int N = 1000010;

bool isprime[N];
int prime[N], phi[N], tot;

int phi2[N];

void getphi(int n)
{
    memset(isprime, true, sizeof(isprime));
    int x;
    tot = 0;
    phi[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        if (isprime[i])
        {
            prime[++tot] = i;
            phi[i] = i - 1;
        }
        for (int j = 1; j <= tot; j++)
        {
            x = i * prime[j];
            if (x > n)
                break;
            isprime[x] = false;
            if (i % prime[j] == 0)
            {
                phi[x] = phi[i] * prime[j];
                break;
            }
            else
                phi[x] = phi[i] * phi[prime[j]];
        }
    }
}
```

### 莫比乌斯函数筛

```cpp
const int N = 1000010;
bool is_prime[N];
int prime[N / 2], mu[N], tot = 0, primesize = 0;

void get_mu()
{
    mu[1] = 1;
    memset(is_prime, true, sizeof(is_prime));

    for (int i = 2; i < N; i++)
    {
        if (is_prime[i])
        {
            prime[++primesize] = i;
            mu[i] = -1;
        }
        for (int j = 1; prime[j] * i < N; j++)
        {
            is_prime[prime[j] * i] = false;
            if (i % prime[j] == 0)
            {
                mu[prime[j] * i] = 0;
                break;
            }
            mu[prime[j] * i] = -mu[i];
        }
    }
}
```



## 矩阵乘法与快速幂

-   N 为矩阵的大小，为了简化先定义为方阵

-   E 为单位矩阵， `init_E()` 可以用来生成单位矩阵

```cpp
const int N = 3;

struct matrix
{
    ll m[N][N];
};

matrix E;

void init_E()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (i == j)
                E.m[i][j] = 1;
            else
                E.m[i][j] = 0;
        }
}

matrix matmul(matrix x, matrix y)
{
    matrix c;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            c.m[i][j] = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                c.m[i][j] = (c.m[i][j] + x.m[i][k] * y.m[k][j]);

    return c;
}

matrix matpow(matrix x, ll y)
{
    matrix ans = E;
    while (y)
    {
        if (y & 1LL)
            ans = matmul(ans, x);
        x = matmul(x, x);
        y >>= 1;
    }
    return ans;
}
```