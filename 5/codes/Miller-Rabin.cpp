#include <iostream>
#include <cstdlib>

using namespace std;

unsigned long long int mul(unsigned long long int a,unsigned long long int b,unsigned long long int n)
{
    unsigned long long int ans = 0;
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

bool Miller_Rabin(unsigned long long int n)
{
    if (n == 2 || n == 3)
    {
        return true;
    }
    if (n < 2 || n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0 || n % 11 == 0)
    {
        return false;
    }
    unsigned long long int m = n - 1, k = 0;
    while (m % 2 == 0)
    {
        m /= 2;
        k++;
    }
    unsigned long long int a = (rand() % (n - 1)) + 1;
    unsigned long long int b = 1;
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
    for (unsigned long long int i = 0; i < k; i++)
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
    unsigned long long int n;
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