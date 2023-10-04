#include <stdexcept>
#include <vector>

using namespace std;

/**
 * @brief implementation of Strassen matrix multiplication
 *
 * @note matrix dimensions: @param a is (m x n) matrix,
 *                          @param b is (n x k) matrix,
 *                     result should be (m x k) matrix
 *
 * @param a representation of matrix a as 1d array
 * @param b representation of matrix b as 1d array
 *
 * For example, to get the entry of matrix a on row i and column j, you could write:
 * \code {.cpp}
 *  int &elem = a[i * n + j]
 * \endcode
 *
 * @return std::vector<int> representation of a dot b as 1d array
 *
 * Constraints:
 *      1 <= m,n,k
 *      no need to worry about int overflow in the entries of the resulting matrix
 */
std::vector<int> matrixMultiplication(
    int                     m,
    int                     n,
    int                     k,
    const std::vector<int> &a,
    const std::vector<int> &b)
{
    // TODO: implement
    throw std::logic_error{ "Strassen multiplication: Not Implemented!" };
}

// benchmark multiplication of squared matrices of size (N x N) (feel free to change)
const vector<long long> Ns{ 1, 5, 25, 128, 512 };

// uniform distribution of matrix entries (feel free to change)
const int entryMin = 0;
const int entryMax = 10;

// don't touch
const bool bT = false;
#include "utils/matrix-multiplication.h"
