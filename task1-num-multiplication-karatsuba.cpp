#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief implementation of karatsuba multiplication algorithm
 *
 * @param s1 - string, which represents the first number
 * @param s2 - string, which represents the second number
 *
 * @return string, which represents multiplication of @param s1 and @param
 * s2
 *
 * Constraints:
 *      1 <= s1.size(), s2.size()
 *      s1 and s2 contains only ascii digits
 *
 * @note @param s1 and @param s2 may contain trailing zeros
 * @note returned value should not contain trailing zeros
 */

string removeLeadingZeros(const string& s) {
    int s_zeros = (int)s.find_first_not_of('0');
    string res = (s_zeros == -1) ? "0" : s.substr(s_zeros, s.size() - s_zeros);
    return res;
}

string addStr(const string &s1, const string &s2) {
    string sliced_s1 = removeLeadingZeros(s1);
    string sliced_s2 = removeLeadingZeros(s2);
    size_t s1_size = sliced_s1.length();
    size_t s2_size = sliced_s2.length();

    if (s1_size > s2_size) {
        while (s1_size > s2_size) {
            sliced_s2.insert(0, "0");
            s2_size++;
        }
    } else {
        while (s2_size > s1_size) {
            sliced_s1.insert(0, "0");
            s1_size++;
        }
    }
    string result {};
    size_t flag = 0;
    for (size_t i = 0; i < s1_size; ++i) {
        size_t num1 = sliced_s1[s1_size - (i+1)] - '0';
        size_t num2 = sliced_s2[s2_size - (i+1)] - '0';
        size_t add = num1 + num2 + flag;
        if (add > 9) {
            flag = 1;
            add %= 10;
        } else {
            flag = 0;
        }
        result.insert(0, to_string(add));
    }
    if (flag) {
        result.insert(0, "1");
    }
    return result;
}

string subtractStr(const string &s1, const string &s2) {
    string sliced_s1 = removeLeadingZeros(s1);
    string sliced_s2 = removeLeadingZeros(s2);
    size_t s1_size = sliced_s1.length();
    size_t s2_size = sliced_s2.length();

    while (s1_size > s2_size) {
        sliced_s2.insert(0, "0");
        s2_size++;
    }

    string result {};
    int flag = 0;
    for (size_t i = 0; i < s1_size; ++i) {
        int num1 = sliced_s1[s1_size - (i+1)] - '0';
        int num2 = sliced_s2[s2_size - (i+1)] - '0';
        int subtr = num1 - num2 - flag;
        if (subtr < 0) {
            flag = 1;
            subtr += 10;
        } else {
            flag = 0;
        }
        result.insert(0, to_string(subtr));
    }
    return result;
}

string numMultiplication(const string &s1, const string &s2) {
    string sliced_s1 = removeLeadingZeros(s1);
    string sliced_s2 = removeLeadingZeros(s2);
    size_t s1_size = sliced_s1.length();
    size_t s2_size = sliced_s2.length();

    if (s1_size > s2_size) {
        while (s1_size > s2_size) {
            sliced_s2.insert(0, "0");
            s2_size++;
        }
    } else {
        while (s2_size > s1_size) {
            sliced_s1.insert(0, "0");
            s1_size++;
        }
    }

    if (s1_size == 1) {
        size_t res = (int)(sliced_s1[0] - '0') * (int)(sliced_s2[0] - '0');
        return to_string(res);
    }

    if ((s1_size % 2) == 1) {
        s1_size++;
        sliced_s1.insert(0, "0");
        sliced_s2.insert(0, "0");
    }

    string a = sliced_s1.substr(0, s1_size / 2);
    string b = sliced_s1.substr(s1_size / 2, s1_size / 2);
    string c = sliced_s2.substr(0, s1_size / 2);
    string d = sliced_s2.substr(s1_size / 2, s1_size / 2);

    string ac = numMultiplication(a, c);
    string bd = numMultiplication(b, d);
    string e = numMultiplication(addStr(a, b), addStr(c, d));

    string abcd = subtractStr(subtractStr(e, ac), bd);

    string fullTenPower (s1_size, '0');
    string halfTenPower (s1_size / 2, '0');

    ac.append(fullTenPower);
    abcd.append(halfTenPower);

    string result = addStr(addStr(ac, bd), abcd);
    return result;
}

// ns to benchmark (feel free to change)
const vector<long long> Ns{ 1, 5, 25, 125, 1024, 2048, 10000, 100000 };

// uniform distribution of digits in s1 and s2 (feel free to change)
const int digitMin = 0;
const int digitMax = 9;

// don't touch
#include "utils/num-multiplication.h"
