#include <iostream>
#include <iomanip>
#include <cstdint>
#include <array>
#include <sstream>
#include <vector>

using namespace std;

const int Nr = 10; // AES-128有10轮加密
array<array<int, 4>, 4> key = {};
array<array<int, 4>, 4> plaintext = {};
array<array<int, 4>, 44> w = {};
array<array<int, 4>, Nr> Rcon = {};
array<array<int, 4>, 4> ciphertext = {};

// AES S-Box
const array<int, 256> SBox = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

//【完结】处理输入
void put_in(array<array<int, 4>, 4>& result, string str)//处理输入
{
    vector<string> pairs;
    for (int i = 0; i < 32; i += 2) {
        string pair = "0x" + str.substr(i, 2);
        pairs.push_back(pair);
    }


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            istringstream iss(pairs[i + j * 4]);
            iss >> hex >> result[i][j];
        }
    }

}

//【完结】初始轮
void InitialRound(array<array<int, 4>, 4>&plaintext, array<array<int, 4>, 4>&key, array<array<int, 4>, 4>&result)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = plaintext[i][j] ^ key[i][j];
        }
    }
}

//【完结】字节代换
void SubBytes(array<array<int, 4>, 4>& input, array<array<int, 4>, 4>& result)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
			result[i][j] = SBox[input[i][j]];
		}
    }
}

//【完结】行移位
void ShiftRows(array<array<int, 4>, 4>& input)
{
    int temp = input[1][0];
    input[1][0] = input[1][1];
    input[1][1] = input[1][2];
    input[1][2] = input[1][3];
    input[1][3] = temp;

    temp = input[2][0];
    input[2][0] = input[2][2];
    input[2][2] = temp;
    temp = input[2][1];
    input[2][1] = input[2][3];
    input[2][3] = temp;

    temp = input[3][0];
    input[3][0] = input[3][3];
    input[3][3] = input[3][2];
    input[3][2] = input[3][1];
    input[3][1] = temp;
}


int mul2(int value) {
    if (value < 0x80) {
        int temp = ((int)value << 1) & 0xff;
        return temp;
    }
    else {
        int temp = ((int)value << 1) & 0xff;
        temp = temp ^ 0x1b;
        return temp;
    }
}

int mul3(int value) {
    return static_cast<int>(mul2(value) ^ value);
}

//【完结】列混淆
void MixColumns(array<array<int, 4>, 4>& input)
{ 
    array<array<int, 4>, 4>temp = {};

    for (int i = 0; i < 4; i++)
    {
        temp[0][i]=mul2(input[0][i]) ^ mul3(input[1][i]) ^ input[2][i] ^ input[3][i];
    }
    for (int i = 0; i < 4; i++)
    {
		temp[1][i] = input[0][i] ^ mul2(input[1][i]) ^ mul3(input[2][i]) ^ input[3][i];
	}
    for (int i = 0; i < 4; i++)
    {
        temp[2][i] = input[0][i] ^ input[1][i] ^ mul2(input[2][i]) ^ mul3(input[3][i]);
    }
    for (int i = 0; i < 4; i++)
    {
		temp[3][i] = mul3(input[0][i]) ^ input[1][i] ^ input[2][i] ^ mul2(input[3][i]);
	}

    input = temp;
}

//【待验证】轮密钥异或
void AddRoundKey(array<array<int, 4>, 4>& input, array<array<int, 4>, 4>& key, array<array<int, 4>, 4>& result)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
			result[i][j] = input[i][j] ^ key[i][j];
		}
    }
}

//【完结】设定轮常数
void SetRocn()
{
    Rcon[0][0] = 0x01;
    Rcon[1][0] = 0x02;
    Rcon[2][0] = 0x04;
    Rcon[3][0] = 0x08;
    Rcon[4][0] = 0x10;
    Rcon[5][0] = 0x20;
    Rcon[6][0] = 0x40;
    Rcon[7][0] = 0x80;
    Rcon[8][0] = 0x1b;
    Rcon[9][0] = 0x36;
}

//【完结】密钥扩展
void KeyExpansion(array<array<int, 4>, 4>& key, array<array<int, 4>, 44>& w)
{
    SetRocn();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            w[i][j] = key[j][i];
        }
    }
    for (int i = 4; i < 44; i++)
    {
        if (i % 4 == 0)
        {
            
            for (int j = 0; j < 4; j++)
            {
                w[i][j] = w[i - 4][j] ^ SBox[w[i - 1][(j + 1) % 4]] ^ Rcon[i / 4 - 1][j];
            }
        }
        else
        {
            for (int j = 0; j < 4; j++)
            {
                w[i][j] = w[i - 4][j] ^ w[i - 1][j];
            }
        }
    }
}

int main() {

    string str_key;
    cin>>str_key;
    //str_key = "2b7e151628aed2a6abf7158809cf4f3c";
    put_in(key,str_key);
    string str_plaintext;
    cin>>str_plaintext;
    //str_plaintext = "3243f6a8885a308d313198a2e0370734";
    put_in(plaintext,str_plaintext);

    KeyExpansion(key, w);
    array<array<int, 4>, 4> temp = {};

    InitialRound(plaintext, key, temp);


    for (int round = 1; round < Nr; round++) {
        SubBytes(temp, temp);
        ShiftRows(temp);
        MixColumns(temp);

        array<array<int, 4>, 4> roundKeytemp = { w[round*4], w[round*4+1], w[round*4 + 2], w[round*4 + 3] };
        array<array<int, 4>, 4> roundKey = { w[round * 4], w[round * 4 + 1], w[round * 4 + 2], w[round * 4 + 3] };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                roundKey[i][j] = roundKeytemp[j][i];
            }
        }
        AddRoundKey(temp, roundKey, temp);
    }

    // Final Round
    SubBytes(temp, temp);
    ShiftRows(temp);

    array<array<int, 4>, 4> roundKeytemp = { w[Nr * 4], w[Nr * 4 + 1], w[Nr * 4 + 2], w[Nr * 4 + 3] };
    array<array<int, 4>, 4> roundKey = { w[Nr * 4], w[Nr * 4 + 1], w[Nr * 4 + 2], w[Nr * 4 + 3] };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            roundKey[i][j] = roundKeytemp[j][i];
        }
    }
    AddRoundKey(temp, roundKey, temp);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ciphertext[i][j] = temp[j][i];
        }
    }
    cout << hex << uppercase << setfill('0');
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << setw(2) << ciphertext[i][j];
        }
    }

    return 0;
}

