#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

/**
 * @brief implementation of grade school multiplication algorithm
 *
 * @param s1 - string, which represents the first number
 * @param s2 - string, which represents the second number
 *
 * @return string, which represents multiplication of @param s1 and @param
 * s2
 *
 * Constraints:
 *      1 <= s1.size(), s2.size() <= 10^5
 *      s1 and s2 contains only ascii digits
 *
 * @note @param s1 and @param s2 may contain trailing zeros
 * @note returned value should not contain trailing zeros
 */

string convert(size_t arr[], int size) {
    string s;
    for (int i = size - 1; i >= 0; i--) {
        s += to_string(arr[i]);
    }
    int s_zeros = (int)s.find_first_not_of('0');
    s = (s_zeros == -1) ? "0" : s.substr(s_zeros, s.size() - s_zeros);
    return s;
}

string removeLeadingZeros(const string& s) {
    int s_zeros = (int)s.find_first_not_of('0');
    string res = (s_zeros == -1) ? "0" : s.substr(s_zeros, s.size() - s_zeros);
    return res;
}

string numMultiplication(const string &s1, const string &s2)
{
    string sliced_s1 = removeLeadingZeros(s1);
    string sliced_s2 = removeLeadingZeros(s2);
    int s1_size = (int) sliced_s1.length();
    int s2_size = (int) sliced_s2.length();
    size_t result_size = s1_size + s2_size;
    auto* result = new size_t[result_size] {};
    size_t flag;
    size_t counter1 = 0;
    size_t counter2 = 0;

    for (int i = s2_size - 1; i >= 0; --i) {
        flag = 0;
        size_t num1 = sliced_s2[i] - '0';

        counter2 = 0;
        for (int j = s1_size - 1; j >= 0; --j) {
            size_t num2 = sliced_s1[j] - '0';
            size_t num = num1 * num2;
            size_t sum = num + result[counter1 + counter2] + flag;

            flag = sum / 10;
            result[counter1 + counter2] = sum%10;

            counter2++;
        }
        if (flag > 0)
            result[counter1 + counter2] += flag;

        counter1++;
    }
    return convert(result, (int)result_size);
}

// ns to benchmark (feel free to change)
const vector<long long> Ns{ 100000, 1000000 };

// uniform distribution of digits in s1 and s2 (feel free to change)
const int digitMin = 0;
const int digitMax = 9;

// don't touch
#include "utils/num-multiplication.h"
