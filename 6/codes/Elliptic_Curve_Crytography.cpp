#include <bits/stdc++.h>
using namespace std;

int64_t mod = 0;

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

class Node
{
public:
    int64_t x, y;
    bool isInf; // whether it is an infinite point
    Node();
    Node(int64_t x, int64_t y);
    Node(int64_t x, int64_t y, bool isInf);
};

Node::Node(): x(0), y(0), isInf(false) {}
Node::Node(int64_t x, int64_t y): x(x), y(y), isInf(false) {}
Node::Node(int64_t x, int64_t y, bool isInf): x(x), y(y), isInf(isInf) {}

class Elliptic_Curve
{
private:
    int64_t a, b, p;

public:
    Elliptic_Curve(int64_t a, int64_t b, int64_t p);
    bool isInverse(const Node &p1, const Node &p2); // check if two points are inverses
    Node add(const Node &p1, const Node &p2);      // perform point addition
    Node addKTimes(Node p, int64_t k);              // perform point multiplication by k
};

Elliptic_Curve::Elliptic_Curve(int64_t a, int64_t b, int64_t p)
{
    this->a = a;
    this->b = b;
    this->p = p;
}

bool Elliptic_Curve::isInverse(const Node &p1, const Node &p2)
{
    return (p1.x - p2.x) % mod == 0 && (p1.y + p2.y) % mod == 0;
}

Node Elliptic_Curve::add(const Node &p1, const Node &p2)
{
    if (p1.isInf)
    {
        return p2;
    }
    else if (p2.isInf)
    {
        return p1;
    }
    else if (isInverse(p1, p2))
    {
        Node tmp(-1, -1, true);
        return tmp;
    }
    else
    {
        Node tmp;
        if ((p1.x - p2.x) % mod == 0)
        {
            int64_t t1 = mul(p1.x, p1.x, mod);
            t1 = mul(3, t1, mod);
            t1 = (t1 + a) % mod;
            int64_t t2 = mul(p1.y, 2, mod);
            t2 = Multiplicative_Inverse(t2, mod);
            int64_t k = mul(t1, t2, mod);
            if (k < 0)
            {
                k += mod;
            }

            int64_t x3 = mul(k, k, mod);
            x3 -= p1.x;
            if (x3 < 0)
            {
                x3 += mod;
            }
            x3 -= p2.x;
            if (x3 < 0)
            {
                x3 += mod;
            }
            tmp.x = x3;

            t1 = p1.x - x3;
            if (t1 < 0)
            {
                t1 += mod;
            }
            int64_t y3 = mul(k, t1, mod);
            y3 -= p1.y;
            if (y3 < 0)
            {
                y3 += mod;
            }
            tmp.y = y3;

            // int64_t k = ((3 * p1.x * p1.x + a) * Multiplicative_Inverse(2 * p1.y, pp) % pp + pp) % pp;
            // int64_t x3 = ((k * k - 2 * p1.x) % pp + pp) % pp;
            // int64_t y3 = ((k * (p1.x - x3) - p1.y) % pp + pp) % pp;

            return tmp;
        }
        else
        {
            int64_t t1 = p2.y - p1.y;
            if (t1 < 0)
            {
                t1 += mod;
            }
            int64_t t2 = p2.x - p1.x;
            if (t2 < 0)
            {
                t2 += mod;
            }
            t2 = Multiplicative_Inverse(t2, mod);
            int64_t k = mul(t1, t2, mod);
            if (k < 0)
            {
                k += mod;
            }

            int64_t x3 = mul(k, k, mod);
            x3 -= p1.x;
            if (x3 < 0)
            {
                x3 += mod;
            }
            x3 -= p2.x;
            if (x3 < 0)
            {
                x3 += mod;
            }
            tmp.x = x3;

            t1 = p1.x - x3;
            if (t1 < 0)
            {
                t1 += mod;
            }
            int64_t y3 = mul(k, t1, mod);
            y3 -= p1.y;
            if (y3 < 0)
            {
                y3 += mod;
            }
            tmp.y = y3;

            // int64_t k = ((p2.y - p1.y) * Multiplicative_Inverse(p2.x - p1.x, pp) % pp + pp) % pp;
            // int64_t x3 = ((k * k - p1.x - p2.x) % pp + pp) % pp;
            // int64_t y3 = ((k * (p1.x - x3) - p1.y) % pp + pp) % pp;

            return tmp;
        }
    }
}

Node Elliptic_Curve::addKTimes(Node p, int64_t k)
{
    Node tmp(-1, -1, true);
    while (k)
    {
        if (k & 1)
        {
            tmp = add(tmp, p);
        }
        p = add(p, p);
        k >>= 1;
    }
    return tmp;
}

int main()
{
    int64_t a, b;
    cin >> a >> b >> mod;
    Elliptic_Curve ecc(a, b, mod);
    int64_t x, y, k;
    cin >> x >> y >> k;
    Node node(x, y, false);
    Node result = ecc.addKTimes(node, k);
    if(result.isInf)
    {
        cout << "-1 -1";
    }
    else
    {
        cout << result.x << " " << result.y;
    }
    return 0;
}
