#include <iostream>
#include <cstdlib>

using namespace std;

uint64_t mul(uint64_t a,uint64_t b,uint64_t n)
{
    uint64_t ans = 0;
    while(b)
    {
        if(b & 1)
        {
            ans = (ans + a) % n;
        }
        a = (a + a) % n;
        b >>= 1;
    }
    return ans;
}

bool Miller_Rabin(uint64_t n)
{
    if (n == 2 || n == 3)
    {
        return true;
    }
    if (n < 2 || n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0 || n % 11 == 0)
    {
        return false;
    }
    uint64_t m = n - 1, k = 0;
    while (m % 2 == 0)
    {
        m /= 2;
        k++;
    }
    uint64_t a = (rand() % (n - 1)) + 1;
    uint64_t b = 1;
    while (m)
    {
        if (m & 1)
        {
            b = mul(b, a, n);
        }
        a = mul(a, a, n);
        m >>= 1;
    }
    if (b == 1)
    {
        return true;
    }
    for (uint64_t i = 0; i < k; i++)
    {
        if (b == n - 1)
        {
            return true;
        }
        b = mul(b, b, n);
    }
    return false;
}
int main()
{
    uint64_t n;
    cin >> n;
    for(int i = 0; i < 10; i++)
    {
        if(!Miller_Rabin(n))
        {
            cout << "No";
            return 0;
        }
    }
    cout << "Yes";
    return 0;
}
