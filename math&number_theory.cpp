#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

#define INF 0x3f3f3f3f
#define EXP 1e-8

#define ll long long



/* Fast pow */
// Without mod
ll fast_pow(ll base, ll exp)
{
    ll t, y;
    t = 1LL;
    y = base;
    while(exp != 0LL)
    {
        if((exp & 1LL) == 1LL)
            t = t * y;
        y = y * y;
        exp = exp >> 1LL;
    }
    return t;
}

// With mod
ll fast_pow(ll base, ll exp, ll mod)
{
    ll t, y;
    t = 1LL;
    y = base;
    while(exp != 0LL)
    {
        if((exp & 1LL) == 1LL)
            t = t * y % mod;
        y = y * y % mod;
        exp = exp >> 1LL;
    }
    return t % mod;
}



/* Get greatest common divisor */
// Euclid: Recursive version
ll gcd(ll a, ll b)
{
    if(b == 0LL)
        return a;
    return gcd(b, a % b);
}

// Euclid: Iterative version
ll gcd1(ll a, ll b)
{
    while(b != 0LL)
    {
        ll r = b;
        b = a % b;
        a = r;
    }
    return a;
}

// Stein
ll gcd2(ll a, ll b)
{
    if(a == 0LL)
        return b;
    if(b == 0LL)
        return a;
    if(a % 2LL == 0LL && b % 2LL == 0LL)
        return 2LL * gcd2(a >> 1LL, b >> 1LL);
    else if(a % 2LL == 0LL)
        return gcd2(a >> 1LL, b);
    else if(b % 2LL == 0LL)
        return gcd2(a, b >> 1LL);
    else
        return gcd2(abs(a - b), min(a, b));
}


/* Least common multiple */
ll lcm(ll a, ll b)
{
    return a / gcd(a, b) * b;
}



/* Judge whether a num is a prime number */
bool is_prime(ll x)
{
    if(x == 1LL)
        return false;
    if(x == 2LL || x == 3LL)
        return true;
    if(x % 6LL != 1LL && x % 6LL != 5LL)
        return false;

    ll s = sqrt(x);
    for(ll i = 5LL; i <= s; i += 6LL)
        if(x % i == 0LL || x % (i + 2LL) == 0LL)
            return false;

    return true;
}



/* Fermat */
// Note that p must be a prime number
ll fermat(ll a, ll p)
{
    return fast_pow(a, p - 2, p);
}



/* Prime factor decomposition */
// Normal version
vector<ll> factorize(ll n)
{
    vector<ll> factors;
    for(ll i = 2LL; i * i <= n; i++)
    {
        while(n != i)
        {
            if(n % i == 0LL)
            {
                factors.push_back(i);
                n = n / i;
            }
            else
                break;
        }
    }

    if(is_prime(n))
        factors.push_back(n);

    return factors;
}



/* Negative number mod*/
// -2 % 5 -> 3
#define mod(a, b)  (((a) % (b) + (b)) % (b))


/* Low Bit*/
// Return the position of lowerest 1 in x's binary
// Index starts from 1
#define lowbit(x)  ((x) & (-x))
