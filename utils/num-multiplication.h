#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <random>
#include <string>
#include <utility>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include "common.h"

extern const std::vector<long long> Ns;
extern const int                    digitMin;
extern const int                    digitMax;
std::string numMultiplication(const std::string &, const std::string &);

namespace Utils::NumMultiplication
{
struct InputData
{
    std::string s1;
    std::string s2;

    void validate() const
    {
        if (s1.empty())
            throw InternalError{ "test case error: string s1 should not be empty" };
        if (s2.empty())
            throw InternalError{ "test case error: string s2 should not be empty" };

        // if (s1.size() > 100'000)
        //     throw InternalError{ "test case error: string s1 too long" };
        // if (s2.size() > 100'000)
        //     throw InternalError{ "test case error: string s2 too long" };

        if (std::count_if(
                s1.begin(),
                s1.end(),
                [](unsigned char c) { return !std::isdigit(c); }) > 0)
        {
            throw InternalError{
                "test case error: found non numeric character in s1"
            };
        }
        if (std::count_if(
                s2.begin(),
                s2.end(),
                [](unsigned char c) { return !std::isdigit(c); }) > 0)
        {
            throw InternalError{
                "test case error: found non numeric character in s2"
            };
        }
    }
};

std::ostream &operator<<(std::ostream &strm, const InputData &rhs)
{
    strm << "InputData(s1=\"" << rhs.s1 << "\", s2=\"" << rhs.s2 << "\")";
    return strm;
}

struct TestCase
{
    InputData inputData;

    std::string expectedResult;

    void validate() const
    {
        inputData.validate();

        if (expectedResult.empty())
        {
            throw InternalError{
                "test case error: expected result string cannot be empty"
            };
        }

        if (std::count_if(
                expectedResult.begin(),
                expectedResult.end(),
                [](unsigned char c) { return !std::isdigit(c); }) > 0)
        {
            throw InternalError{
                "test case error: found non numeric character in expected result string"
            };
        }
    }

    static TestCase fromTestIndex(size_t idx)
    {
        TestCase testCase;

        auto [inContent, outContent] = getTest(idx);

        auto in  = std::stringstream{ std::move(inContent) };
        auto out = std::stringstream{ std::move(outContent) };

        in >> testCase.inputData.s1;
        in >> testCase.inputData.s2;

        out >> testCase.expectedResult;

        if (in.fail() || out.fail())
            throw InternalError{ "Wrong test format" };

        testCase.validate();

        return testCase;
    }
};

std::ostream &operator<<(std::ostream &strm, const TestCase &rhs)
{
    strm << "TestCase(inputData=" << rhs.inputData << ", expectedResult=\""
         << rhs.expectedResult << "\")";
    return strm;
}

static void BM_NumMultiplication(benchmark::State &state)
{
    std::mt19937 generator{ 173 };

    static constexpr std::array<char, 10> digits{ '0', '1', '2', '3', '4',
                                                  '5', '6', '7', '8', '9' };
    auto distrDigit = std::uniform_int_distribution<int>{ digitMin, digitMax };

    const auto n = static_cast<long long>(state.range(0));

    for (auto _ : state)
    {
        state.PauseTiming();

        auto s1 = std::string{};
        auto s2 = std::string{};

        for (auto s : { &s1, &s2 })
        {
            s->reserve(n);
            for (long long i = 0; i < n; ++i)
            {
                auto digit     = distrDigit(generator);
                auto charDigit = digits.at(digit);
                s->push_back(charDigit);
            }
        }

        auto inp = InputData{ std::move(s1), std::move(s2) };
        inp.validate();

        state.ResumeTiming();

        auto res = numMultiplication(inp.s1, inp.s2);
        ::benchmark::DoNotOptimize(res);
    }
}

BENCHMARK(BM_NumMultiplication)
    ->Apply(
        [](benchmark::internal::Benchmark *b)
        {
            for (const auto &n : Ns)
                b->Arg(n);

            b->Name("NumMultiplication/" + std::string{ HW1_METHOD_NAME_STR });

            benchmark::AddCustomContext("distr/digitMin", std::to_string(digitMin));
            benchmark::AddCustomContext("distr/digitMax", std::to_string(digitMax));
        });

class HW1_METHOD_NAME : public testing::TestWithParam<size_t>
{
};

TEST_P(HW1_METHOD_NAME, ImplementationCorrectness)
{
    const auto testCase = TestCase::fromTestIndex(GetParam());

    const auto &inputData      = testCase.inputData;
    const auto  expectedResult = testCase.expectedResult;

    const auto actualResult = numMultiplication(inputData.s1, inputData.s2);

    ASSERT_EQ(expectedResult, actualResult) << testCase;
}

INSTANTIATE_TEST_SUITE_P(
    NumMultiplication,
    HW1_METHOD_NAME,
    ::testing::Range<size_t>(size_t{ 1 }, size_t{ testsNum() + 1 }));

}    // namespace Utils::NumMultiplication
