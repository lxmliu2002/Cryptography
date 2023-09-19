import numpy as np
from collections import defaultdict

# 计算字母频率
def calculate_letter_frequency(text):
    length = len(text)
    letter_count = defaultdict(int)
    for letter in text:
        letter_count[letter] += 1
    return letter_count, length

# 计算重合指数
def calculate_index_of_coincidence(text, letters):
    letter_count, length = calculate_letter_frequency(text)
    sum_term = sum([letter_count[letter] * (letter_count[letter] - 1) for letter in letters])
    return sum_term / (length * (length - 1))

# 计算最佳m值
def calculate_optimal_m(text, letters):
    m_values = [0.0]
    for m in range(1, 11):
        print(f'm={m}', end=' ')
        substrings = ["" for _ in range(m)]
        for i, letter in enumerate(text):
            substrings[i % m] += letter
        sum_ic = 0.0
        for substring in substrings:
            ic = calculate_index_of_coincidence(substring, letters)
            sum_ic += ic
            print(f'{ic:.5f}', end=' ')
        print()
        m_values.append(sum_ic / m)
    optimal_m = np.array(m_values).argmax(axis=0)
    print(f'密钥长度 m = {optimal_m}')
    return optimal_m

# 计算密钥
def calculate_key(text, letters, letter_frequencies):
    m = calculate_optimal_m(text, letters)
    substrings = ["" for _ in range(m)]
    for i, letter in enumerate(text):
        substrings[i % m] += letter
    key = []
    for substring in substrings:
        print(substring + ', ', end='')
        letter_count, length = calculate_letter_frequency(substring)
        Mg_values = []
        for g in range(26):
            Mg = 0.0
            for i in range(26):
                Mg += letter_frequencies[i] * letter_count[letters[(i + g) % 26]]
            Mg /= length
            Mg_values.append(Mg)
        k = np.abs(np.array(Mg_values) - np.array(0.065)).argmin(axis=0)
        key.append(k)
        print(f'k = {k}, Mg({k}) = {Mg_values[k]:.5f}')
    return key


ciphertext = "KCCPKBGUFDPHOTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKORACOCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKODYHJVDAHCTRISVSKCGCZOODZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZONRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSOESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFOIYCWHJVLNHIOIBTKHJVNPIST"
letters = [chr(ord('A') + i) for i in range(26)]
letter_frequencies = [0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091, 0.028, 0.010, 0.023, 0.001, 0.020, 0.001]

# 计算密钥
key = calculate_key(ciphertext, letters, letter_frequencies)
print(f'Key = {key}')

# 解密文本
def decrypt_text(ciphertext, key, letters):
    plaintext = ""
    n = len(key)
    for i, letter in enumerate(ciphertext):
        letter_value = ord(letter) - ord('A')
        k = key[i % n]
        plaintext += letters[(letter_value - k + 26) % 26]
    return plaintext.lower()
plaintext = decrypt_text(ciphertext, key, letters)
print(f'明文: {plaintext}')
