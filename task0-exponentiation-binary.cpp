#include <stdexcept>
#include <vector>

using namespace std;

/**
 * @brief implementation of binary exponentiation
 *
 * @note it's guaranteed that a * a and m * m will fit into long long
 *
 * @note @param a is guaranteed to be non-negative
 * @note @param n is guaranteed to be non-negative
 * @note @param m is guaranteed to be positive
 *
 * @return (a^n)%m
 */
long long exponentiation(long long a, long long n, long long m)
{
    // TODO: implement
    // throw std::logic_error{"binary exponentiation: Not Implemented!"};

    if (n == 0)
        return 1;

    if (n % 2 == 0)
    {
        auto res = exponentiation(a, n / 2, m);
        return (res * res) % m;
    }
    else
    {
        return (a * exponentiation(a, n - 1, m)) % m;
    }
}

// ns to benchmark (feel free to change)
const vector<long long> Ns{ 1, 5, 25, 125, 1024, 2048, 1000000, (long long)1e9 };

// uniform distributions of as and ms in benchmarks (feel free to change)
const long long aMin = 0;
const long long aMax = 100;
const long long mMin = 100;
const long long mMax = 1000000000;

// don't touch
#include "utils/exponentiation.h"
