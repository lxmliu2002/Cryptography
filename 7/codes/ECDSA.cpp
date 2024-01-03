#include <iostream>
using namespace std;

int64_t mul(int64_t a, int64_t b, int64_t n)
{
    int64_t result = 0;
    while (b)
    {
        if (b & 1)
        {
            result = (result + a) % n;
        }
        a = (a + a) % n;
        b >>= 1;
    }
    return result;
}

int64_t Multiplicative_Inverse(int64_t a, int64_t p)
{
    int64_t s, t;
    int64_t a0 = a;
    int64_t r0 = a0;
    int64_t r1 = p;
    int64_t s0 = 1;
    int64_t t0 = 0;
    int64_t s1 = 0;
    int64_t t1 = 1;
    int64_t r = r0 % r1;
    int64_t q = (r0 - r) / r1;
    while (r)
    {
        s = s0 - q * s1;
        t = t0 - q * t1;
        s0 = s1;
        t0 = t1;
        s1 = s;
        t1 = t;

        r0 = r1;
        r1 = r;
        r = r0 % r1;
        q = (r0 - r) / r1;
    }
    if (r1 != 1)
    {
        return 0;
    }
    return (s + p) % p;
}

int64_t Hash(int64_t x, int64_t base, int64_t q)
{
    int64_t result = 1;
    while (x)
    {
        if (x & 1)
        {
            result = mul(result, base, q);
        }
        base = mul(base, base, q);
        x >>= 1;
    }
    return result;
}

class Node
{
public:
    int64_t x, y;
    bool isInf;
    Node();
    Node(int64_t x, int64_t y);
    Node(int64_t x, int64_t y, bool isInf);
};

Node::Node() : x(0), y(0), isInf(false) {}
Node::Node(int64_t x, int64_t y) : x(x), y(y), isInf(false) {}
Node::Node(int64_t x, int64_t y, bool isInf) : x(x), y(y), isInf(isInf) {}

bool isInverse(Node p1, Node p2, int64_t p)
{
    return (p1.x - p2.x) % p == 0 && (p1.y + p2.y) % p == 0;
}

Node add(Node p1, Node p2, int64_t a, int64_t p)
{
    if (p1.isInf)
    {
        return p2;
    }
    else if (p2.isInf)
    {
        return p1;
    }
    else if (isInverse(p1, p2, p))
    {
        Node tmp(-1, -1, true);
        return tmp;
    }
    else if ((p1.x - p2.x) % p == 0)
    {
        Node tmp;
        int64_t t1 = mul(p1.x, p1.x, p);
        t1 = mul(3, t1, p);
        t1 = (t1 + a) % p;
        int64_t t2 = mul(p1.y, 2, p);
        t2 = Multiplicative_Inverse(t2, p);
        int64_t k = mul(t1, t2, p);
        if (k < 0)
        {
            k += p;
        }

        int64_t x3 = mul(k, k, p);
        x3 -= p1.x;
        if (x3 < 0)
        {
            x3 += p;
        }
        x3 -= p2.x;
        if (x3 < 0)
        {
            x3 += p;
        }
        tmp.x = x3;

        t1 = p1.x - x3;
        if (t1 < 0)
        {
            t1 += p;
        }
        int64_t y3 = mul(k, t1, p);
        y3 -= p1.y;
        if (y3 < 0)
        {
            y3 += p;
        }
        tmp.y = y3;

        // int64_t k = ((3 * p1.x * p1.x + a) * Multiplicative_Inverse(2 * p1.y, pp) % pp + pp) % pp;
        // int64_t x3 = ((k * k - 2 * p1.x) % pp + pp) % pp;
        // int64_t y3 = ((k * (p1.x - x3) - p1.y) % pp + pp) % pp;

        return tmp;
    }
    else
    {
        Node tmp;
        int64_t t1 = p2.y - p1.y;
        if (t1 < 0)
        {
            t1 += p;
        }
        int64_t t2 = p2.x - p1.x;
        if (t2 < 0)
        {
            t2 += p;
        }
        t2 = Multiplicative_Inverse(t2, p);
        int64_t k = mul(t1, t2, p);
        if (k < 0)
        {
            k += p;
        }

        int64_t x3 = mul(k, k, p);
        x3 -= p1.x;
        if (x3 < 0)
        {
            x3 += p;
        }
        x3 -= p2.x;
        if (x3 < 0)
        {
            x3 += p;
        }
        tmp.x = x3;

        t1 = p1.x - x3;
        if (t1 < 0)
        {
            t1 += p;
        }
        int64_t y3 = mul(k, t1, p);
        y3 -= p1.y;
        if (y3 < 0)
        {
            y3 += p;
        }
        tmp.y = y3;

        // int64_t k = ((p2.y - p1.y) * Multiplicative_Inverse(p2.x - p1.x, pp) % pp + pp) % pp;
        // int64_t x3 = ((k * k - p1.x - p2.x) % pp + pp) % pp;
        // int64_t y3 = ((k * (p1.x - x3) - p1.y) % pp + pp) % pp;

        return tmp;
    }
}

Node addKTimes(Node pp, int64_t k, int64_t a, int64_t p)
{
    Node tmp(-1, -1, true);
    while (k)
    {
        if (k & 1)
        {
            tmp = add(tmp, pp, a, p);
        }
        pp = add(pp, pp, a, p);
        k >>= 1;
    }
    return tmp;
}

int main()
{
    int64_t a, b, p;
    a = 1;
    b = 6;
    p = 11;
    int64_t x0, y0;
    cin >> x0 >> y0;
    int64_t m, x, k;
    cin >> m >> x >> k;
    Node node(x0, y0, false);
    Node kA = addKTimes(node, k, a, p);
    int64_t q = 13;
    int64_t r = kA.x % q;
    int64_t k_Inverse = Multiplicative_Inverse(k, q);
    int64_t tmp = mul(r, m, q);
    tmp += Hash(x, 2, q);
    int64_t s = mul(k_Inverse, tmp % q, q);
    cout << r << " " << s;

    return 0;
}