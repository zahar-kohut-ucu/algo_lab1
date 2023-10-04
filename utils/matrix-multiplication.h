#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <limits>
#include <random>
#include <string>
#include <utility>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include "common.h"

extern const std::vector<long long> Ns;
extern const int                    entryMin;
extern const int                    entryMax;
extern const bool                   bT;

std::vector<int> matrixMultiplication(
    int                     m,
    int                     n,
    int                     k,
    const std::vector<int> &a,
    const std::vector<int> &b);

namespace Utils::MatrixMultiplication
{
std::vector<int> transposeMatrix(const std::vector<int> &matrix, int rows, int cols)
{
    std::vector<int> transposed(rows * cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            transposed[j * rows + i] = matrix[i * cols + j];
        }
    }

    return transposed;
}
void printMatrix(
    std::ostream           &strm,
    const std::vector<int> &matrix,
    int                     rows,
    int                     cols)
{
    auto idx = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            strm << matrix[idx++] << " ";
        strm << std::endl;
    }
}

struct InputData
{
    int m;
    int n;
    int k;

    std::vector<int> a;
    std::vector<int> b;

    void validate() const
    {
        if (m < 1)
            throw InternalError{ "test case error: m should be >= 1" };
        if (n < 1)
            throw InternalError{ "test case error: n should be >= 1" };
        if (k < 1)
            throw InternalError{ "test case error: k should be >= 1" };

        // if (m > 1024 || n > 1024)
        //     throw InternalError{ "test case error: matrix a too big" };
        // if (n > 1024 || k > 1024)
        //     throw InternalError{ "test case error: matrix b too big" };

        if (a.size() != m * n)
            throw InternalError{ "test case error: wrong size of vector a" };
        if (b.size() != n * k)
            throw InternalError{ "test case error: wrong size of vector b" };
    }
};

std::ostream &operator<<(std::ostream &strm, const InputData &rhs)
{
    strm << "InputData(m=" << rhs.m << ", n=" << rhs.n << ", k=" << rhs.k
         << ", a=" << std::endl;

    printMatrix(strm, rhs.a, rhs.m, rhs.n);

    strm << "b=" << std::endl;

    if (::bT)
        printMatrix(strm, rhs.b, rhs.k, rhs.n);
    else
        printMatrix(strm, rhs.b, rhs.n, rhs.k);

    strm << ")";
    return strm;
}

struct TestCase
{
    InputData inputData;

    std::vector<int> expectedResult;

    void validate() const
    {
        inputData.validate();

        if (expectedResult.size() != inputData.m * inputData.k)
            throw InternalError{
                "test case error: wrong size of expected matrix c"
            };
    }

    static TestCase fromTestIndex(size_t idx)
    {
        TestCase testCase;

        auto [inContent, outContent] = getTest(idx);

        auto in  = std::stringstream{ std::move(inContent) };
        auto out = std::stringstream{ std::move(outContent) };

        in >> testCase.inputData.m >> testCase.inputData.n;
        testCase.inputData.a.resize(testCase.inputData.m * testCase.inputData.n);
        for (int i = 0; i < testCase.inputData.m * testCase.inputData.n; ++i)
            in >> testCase.inputData.a[i];

        in >> testCase.inputData.n >> testCase.inputData.k;
        testCase.inputData.b.resize(testCase.inputData.n * testCase.inputData.k);
        for (int i = 0; i < testCase.inputData.n * testCase.inputData.k; ++i)
            in >> testCase.inputData.b[i];

        if (::bT)
        {
            testCase.inputData.b = transposeMatrix(
                testCase.inputData.b, testCase.inputData.n, testCase.inputData.k);
        }

        testCase.expectedResult.resize(testCase.inputData.m * testCase.inputData.k);
        for (int i = 0; i < testCase.inputData.m * testCase.inputData.k; ++i)
            out >> testCase.expectedResult[i];

        if (in.fail() || out.fail())
            throw InternalError{ "Wrong test format" };

        testCase.validate();

        return testCase;
    }
};

std::ostream &operator<<(std::ostream &strm, const TestCase &rhs)
{
    strm << "TestCase(inputData=" << rhs.inputData << ", expectedResult=\""
         << std::endl;

    printMatrix(strm, rhs.expectedResult, rhs.inputData.m, rhs.inputData.k);

    strm << ")";

    return strm;
}

static void BM_NumMultiplication(benchmark::State &state)
{
    std::mt19937 generator{ 173 };

    auto distrEntry = std::uniform_int_distribution<int>{ entryMin, entryMax };

    const auto n = static_cast<int>(state.range(0));

    for (auto _ : state)
    {
        state.PauseTiming();

        auto a = std::vector<int>{};
        auto b = std::vector<int>{};

        for (auto m : { &a, &b })
        {
            m->reserve(n * n);

            for (int i = 0; i < n * n; ++i)
                m->push_back(distrEntry(generator));
        }

        auto inp = InputData{ n, n, n, std::move(a), std::move(b) };
        inp.validate();

        state.ResumeTiming();

        auto res = matrixMultiplication(n, n, n, inp.a, inp.b);
        ::benchmark::DoNotOptimize(res);
    }
}

BENCHMARK(BM_NumMultiplication)
    ->Apply(
        [](benchmark::internal::Benchmark *b)
        {
            for (const auto &n : Ns)
                b->Arg(n);

            b->Name("MatrixMultiplication/" + std::string{ HW1_METHOD_NAME_STR });

            benchmark::AddCustomContext("distr/entryMin", std::to_string(entryMin));
            benchmark::AddCustomContext("distr/entryMax", std::to_string(entryMax));
        });

class HW1_METHOD_NAME : public testing::TestWithParam<size_t>
{
};

TEST_P(HW1_METHOD_NAME, ImplementationCorrectness)
{
    const auto testCase = TestCase::fromTestIndex(GetParam());

    const auto &inputData      = testCase.inputData;
    const auto  expectedResult = testCase.expectedResult;

    const auto actualResult = matrixMultiplication(
        inputData.m, inputData.n, inputData.k, inputData.a, inputData.b);

    ASSERT_EQ(expectedResult, actualResult) << testCase;
}

INSTANTIATE_TEST_SUITE_P(
    MatrixMultiplication,
    HW1_METHOD_NAME,
    ::testing::Range<size_t>(size_t{ 1 }, size_t{ testsNum() + 1 }));

}    // namespace Utils::MatrixMultiplication
