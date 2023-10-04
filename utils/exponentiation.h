#pragma once

#include <cmath>
#include <functional>
#include <limits>
#include <ostream>
#include <random>
#include <sstream>
#include <string>
#include <utility>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include "common.h"

extern const std::vector<long long> Ns;
extern const long long              aMin;
extern const long long              aMax;
extern const long long              mMin;
extern const long long              mMax;
long long exponentiation(long long, long long, long long);

namespace Utils::Exponentiation
{
struct InputData
{
    long long a;
    long long n;
    long long m;

    void validate() const
    {
        static const auto maxSafeLongLong = static_cast<long long>(
            std::sqrt(static_cast<double>(std::numeric_limits<long long>::max())));

        if (a < 0)
            throw InternalError{ "test case error: a should be non-negative" };
        if (n < 0)
            throw InternalError{ "test case error: n should be non-negative" };
        if (m <= 1)
            throw InternalError{ "test case error: m should be >= 2" };

        if (a > maxSafeLongLong)
            throw InternalError{
                "test case error: a * a does not fit into long long"
            };
        if (m > maxSafeLongLong)
            throw InternalError{
                "test case error: m * m does not fit into long long"
            };
    }
};

std::ostream &operator<<(std::ostream &strm, const InputData &rhs)
{
    strm << "InputData(a=" << rhs.a << ", n=" << rhs.n << ", m=" << rhs.m << ")";
    return strm;
}

struct TestCase
{
    InputData inputData;

    long long expectedResult;

    void validate() const
    {
        inputData.validate();
        if (expectedResult >= inputData.m)
        {
            throw InternalError{ "test case error: expectedResult " +
                                 std::to_string(expectedResult) +
                                 " is greater than modulo m " +
                                 std::to_string(inputData.m) };
        }
    }

    static TestCase fromTestIndex(size_t idx)
    {
        TestCase testCase;

        auto [inContent, outContent] = getTest(idx);

        auto in  = std::stringstream{ std::move(inContent) };
        auto out = std::stringstream{ std::move(outContent) };

        in >> testCase.inputData.a;
        in >> testCase.inputData.n;
        in >> testCase.inputData.m;

        out >> testCase.expectedResult;

        if (in.fail() || out.fail())
            throw InternalError{ "Wrong test format" };

        testCase.validate();

        return testCase;
    }
};

std::ostream &operator<<(std::ostream &strm, const TestCase &rhs)
{
    strm << "TestCase(inputData=" << rhs.inputData
         << ", expectedResult=" << rhs.expectedResult << ")";
    return strm;
}

static void BM_Exponentiation(benchmark::State &state)
{
    std::mt19937 generator{ 173 };

    auto distrA = std::uniform_int_distribution<long long>{ aMin, aMax };
    auto distrM = std::uniform_int_distribution<long long>{ mMin, mMax };

    const auto n = static_cast<long long>(state.range(0));

    for (auto _ : state)
    {
        state.PauseTiming();

        auto inp = InputData{ distrA(generator), n, distrM(generator) };
        inp.validate();

        state.ResumeTiming();

        auto res = ::exponentiation(inp.a, inp.n, inp.m);
        ::benchmark::DoNotOptimize(res);
    }
}

BENCHMARK(BM_Exponentiation)
    ->Apply(
        [](benchmark::internal::Benchmark *b)
        {
            for (const auto &n : Ns)
                b->Arg(n);

            b->Name("Exponentiation/" + std::string{ HW1_METHOD_NAME_STR });

            benchmark::AddCustomContext("distr/aMin", std::to_string(aMin));
            benchmark::AddCustomContext("distr/aMax", std::to_string(aMax));

            benchmark::AddCustomContext("distr/mMin", std::to_string(mMin));
            benchmark::AddCustomContext("distr/mMax", std::to_string(mMax));
        });

class HW1_METHOD_NAME : public testing::TestWithParam<size_t>
{
};

TEST_P(HW1_METHOD_NAME, ImplementationCorrectness)
{
    const auto testCase = TestCase::fromTestIndex(GetParam());

    const auto &inputData      = testCase.inputData;
    const auto  expectedResult = testCase.expectedResult;

    const auto actualResult = exponentiation(inputData.a, inputData.n, inputData.m);

    ASSERT_EQ(expectedResult, actualResult) << testCase;
}

INSTANTIATE_TEST_SUITE_P(
    Exponentiation,
    HW1_METHOD_NAME,
    ::testing::Range<size_t>(size_t{ 1 }, size_t{ testsNum() + 1 }));

}    // namespace Utils::Exponentiation
