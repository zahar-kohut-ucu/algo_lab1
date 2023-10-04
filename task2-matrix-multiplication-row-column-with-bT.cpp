#include <stdexcept>
#include <vector>

using namespace std;

/**
 * @brief implementation of row-column matrix multiplication with b transposed
 *
 * @note matrix dimensions: @param a is (m x n) matrix,
 *                          @param b is (k x n) matrix,
 *                     result should be (m x k) matrix
 *
 * @note you should calculate the dot product of a and b transposed for the
 * dimensions to make sense
 *
 * @param a representation of matrix a as 1d array
 * @param b representation of matrix b as 1d array
 *
 * For example, to get the entry of matrix a on row i and column j, you could write:
 * \code {.cpp}
 *  int &elem = a[i * n + j]
 * \endcode
 *
 * @return std::vector<int> representation of (a) dot (b transposed) as 1d array
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
    throw std::logic_error{
        "row-column multiplication (with b transposed): Not Implemented!"
    };
}

// benchmark multiplication of squared matrices of size (N x N) (feel free to change)
const vector<long long> Ns{ 1, 5, 25, 128, 256, 512, 1024 };

// uniform distribution of matrix entries (feel free to change)
const int entryMin = 0;
const int entryMax = 10;

// don't touch
const bool bT = true;
#include "utils/matrix-multiplication.h"
