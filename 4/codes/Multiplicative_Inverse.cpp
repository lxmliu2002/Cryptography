#include <iostream>
using namespace std;

int Multiplicative_Inverse(int a, int p)
{
    int s, t;
    int a0 = a;
    int r0 = a0;
    int r1 = p;
    int s0 = 1;
    int t0 = 0;
    int s1 = 0;
    int t1 = 1;
    int r = r0 % r1;
    int q = (r0 - r) / r1;
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
    if (r1 != 1) return 0;
    if (s < 0)
    {
        s += p;
    }
    return s;
}

int main() {
    int a, p;
    cin >> a >> p;
    cout << Multiplicative_Inverse(a, p);
    return 0;
}