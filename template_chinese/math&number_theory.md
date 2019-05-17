### 数学/数论模版

需要注意：部分函数可能需要用到 `cmath` 等头文件内的函数。

模版中的 `ll` 全部代表 `long long` 。

建议在 c++ 文件前声明：

```cpp
#define ll long long
// 或者
typedef long long ll
```



#### 快速幂

##### 不带模
```cpp
ll fast_pow(ll base, ll exp)
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

##### 取模

```cpp
ll fast_pow(ll base, ll exp, ll mod)
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



#### GCD (最大公约数)

##### 欧几里得算法 (递归版)

```cpp
ll gcd(ll a, ll b)
{
    if (b == 0LL)
        return a;
    return gcd(b, a % b);
}
```

##### 欧几里得算法 (迭代版)

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

##### Stein 算法

```cpp
ll gcd(ll a, ll b)
{
    if (a == 0LL)
        return b;
    if (b == 0LL)
        return a;
    if (a % 2LL == 0LL && b % 2LL == 0LL)
        return 2LL * gcd2(a >> 1LL, b >> 1LL);
    else if (a % 2LL == 0LL)
        return gcd2(a >> 1LL, b);
    else if (b % 2LL == 0LL)
        return gcd2(a, b >> 1LL);
    else
        return gcd2(abs(a - b), min(a, b));
}
```



#### LCM (最小公倍数)

```cpp
ll lcm(ll a, ll b)
{
    return a / gcd(a, b) * b;
}
```



#### 判断素数

```cpp
bool is_prime(ll x)
{
    if (x == 1LL)
        return false;
    if (x == 2LL || x == 3LL)
        return true;
    if (x % 6LL != 1LL && x % 6LL != 5LL)
        return false;

    ll s = sqrt(x);
    for (ll i = 5LL; i <= s; i += 6LL)
        if (x % i == 0LL || x % (i + 2LL) == 0LL)
            return false;

    return true;
}
```



#### 求逆元

##### 费马小定理

* 注意： `p` 一定要是质数

```cpp
ll fermat(ll a, ll p)
{
    return fast_pow(a, p - 2, p);
}
```

##### exgcd (拓展欧几里得法)

* 注意： `p` 和 `a` 一定要互质

```cpp
ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if (a == 0 && b == 0)
        return -1LL;
    if (b == 0)
    {
        x = 1LL;
        y = 0LL;
        return a;
    }
    ll d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

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



#### 质因子分解

##### 普通版本

```cpp
vector<ll> factorize(ll n)
{
    vector<ll> factors;
    for (ll i = 2LL; i * i <= n; i++)
    {
        while (n != i)
        {
            if (n % i == 0LL)
            {
                factors.push_back(i);
                n = n / i;
            }
            else
                break;
        }
    }

    if (is_prime(n))
        factors.push_back(n);

    return factors;
}
```



#### 负数取模

* 将负数取模转化为正数结果

* -2 % 5 -> 3

```cpp
#define mod(a, b) (((a) % (b) + (b)) % (b))
```



#### 最低 1 位

* 返回 `x` 的二进制中最后一个 `1` 的位置。

* 下标从 1 开始，从右往左标记。

```cpp
#define lowbit(x) ((x) & (-x))
```



#### 质数表

##### 线性筛

* 时间复杂度：O(n)

```cpp
const int MAXN = 1000010;
int prime[MAXN], primesize;
bool isprime[MAXN];
void get_prime()
{
    memset(isprime, true, sizeof(isprime));
    isprime[1] = false;
    for(int i = 2; i <= MAXN; i++)
    {
        if(isprime[i])
            prime[++primesize]=i;
        for(int j = 1; j <= primesize && i * prime[j] <= MAXN; j++)
        {
            isprime[i * prime[j]] = false;
            if(i % prime[j] == 0)
                break;
        }
    }
}
```