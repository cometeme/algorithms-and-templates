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