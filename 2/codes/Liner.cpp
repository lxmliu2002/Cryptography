#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
using namespace std;
using namespace chrono;

int S[16] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
int x[16] = {};//����
int y[16] = {};//����
int Count[16][16] = {};//������

void DecimalToBinary(int decimal, int* binary, int num);

int main()
{
    int S1[16] = {};//S�е���
    for (int i = 0; i < 16; i++)
    {
        S1[S[i]] = i;
    }

    ifstream input(".\\data\\pairs.txt");

    int L1[4] = {};//������Կ1 
    int L2[4] = {};//������Կ2
    int v[16] = {};
    int u[16] = {};

    int n = 0;//�������ĶԸ���
    cout << "��������Ҫ���������ĶԸ�����" << endl;
    cin >> n;

    auto start = high_resolution_clock::now(); //��ʼʱ��

    for (int i = 0; i < n; i++)
    {
        string X, Y;
        input >> X >> Y;

        int len = X.length();
        for (int j = 0; j < len; j++) x[j] = X[j] - '0';

        len = Y.length();
        for (int j = 0; j < len; j++) y[j] = Y[j] - '0';

        for (int j = 0; j < 16; j++)
        {
            for (int k = 0; k < 16; k++)
            {
                //for(L_1,L_2) <- (0,0) to (F,F)
                DecimalToBinary(j, L1, 3);
                DecimalToBinary(k, L2, 3);

                //L_1��y_{<2>}���
                v[4] = L1[0] ^ y[4];
                v[5] = L1[1] ^ y[5];
                v[6] = L1[2] ^ y[6];
                v[7] = L1[3] ^ y[7];
                //L_2��y_{<4>}���
                v[12] = L2[0] ^ y[12];
                v[13] = L2[1] ^ y[13];
                v[14] = L2[2] ^ y[14];
                v[15] = L2[3] ^ y[15];

                int temp1 = v[4] * pow(2, 3) + v[5] * pow(2, 2) + v[6] * pow(2, 1) + v[7] * pow(2, 0);
                int temp2 = S1[temp1];//S�е�������
                DecimalToBinary(temp2, u, 7);

                temp1 = v[12] * pow(2, 3) + v[13] * pow(2, 2) + v[14] * pow(2, 1) + v[15] * pow(2, 0);
                temp2 = S1[temp1];//S�е�������
                DecimalToBinary(temp2, u, 15);

                int z = x[4] ^ x[6] ^ x[7] ^ u[5] ^ u[7] ^ u[13] ^ u[15];

                if (z == 0) Count[j][k]++;
            }
        }
    }

    auto stop = high_resolution_clock::now(); //����ʱ��
    auto duration = duration_cast<microseconds>(stop - start); //ִ��ʱ��

    input.close();

    int max = -1;
    int LL1 = 0, LL2 = 0;//��¼����Count[i][j]��Ӧ��L1��L2
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            Count[i][j] = abs(Count[i][j] - n / 2);
            if (Count[i][j] > max)
            {
                max = Count[i][j];
                LL1 = i;
                LL2 = j;
            }
        }
    }

    cout << "maxkey:" << endl;
    DecimalToBinary(LL1, L1, 3);
    for (int i = 0; i < 4; i++)
    {
        cout << L1[i];
    }
    cout << " ";
    DecimalToBinary(LL2, L2, 3);
    for (int i = 0; i < 4; i++)
    {
        cout << L2[i];
    }

    cout << endl << "time��" << endl << duration.count() << " ΢��" << endl;

    system("pause");
    return 0;
}

void DecimalToBinary(int decimal, int* binary, int num)
{
    int i = num - 3;//�������λ
    while (decimal > 0)
    {
        binary[num] = decimal % 2;
        decimal /= 2;
        num--;
    }
    while (num >= i)//��λ��0
    {
        binary[num] = 0;
        num--;
    }
}