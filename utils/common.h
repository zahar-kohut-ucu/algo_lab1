#pragma once

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

namespace Utils
{
/**
 * @brief this error indicates the error in template implementation. It is expected
 * to not to be thrown
 */
class InternalError : public std::exception
{
public:
    InternalError(std::string message) : _message{ std::move(message) } {}

    const char *what() const noexcept override { return _message.data(); }

private:
    std::string _message;
};

size_t testsNum()
{
    const std::filesystem::path testsFolder{ HW1_TESTS_FOLDER };

    if (!std::filesystem::is_directory(testsFolder))
    {
        throw InternalError{ "Could not find tests folder: " +
                             testsFolder.string() };
    }

    size_t res = 0;

    for (const auto &item : std::filesystem::directory_iterator(testsFolder))
    {
        const auto path = item.path();

        if (!std::filesystem::is_regular_file(path))
        {
            throw InternalError{ "Expected " + path.string() +
                                 " to be a regular file" };
        }

        const auto basename = path.filename().stem();

        auto strm = std::stringstream{ basename.string() };

        size_t idx;
        strm >> idx;

        if (strm.fail())
        {
            throw InternalError{ "Test index " + basename.string() +
                                 " not understood" };
        }

        if (idx == 0)
        {
            throw InternalError{ "Unexpected test index 0" };
        }

        auto inFile  = testsFolder / (std::to_string(idx) + ".in");
        auto outFile = testsFolder / (std::to_string(idx) + ".out");

        for (const auto file : { inFile, outFile })
        {
            if (!std::filesystem::is_regular_file(file))
            {
                throw InternalError{ "Could not find test file: " + file.string() };
            }
        }

        res = std::max(res, idx);
    }

    return res;
}

std::tuple<std::string, std::string> getTest(size_t idx)
{
    const std::filesystem::path testsFolder{ HW1_TESTS_FOLDER };

    auto inFilePath  = testsFolder / (std::to_string(idx) + ".in");
    auto outFilePath = testsFolder / (std::to_string(idx) + ".out");

    for (const auto file : { inFilePath, outFilePath })
    {
        if (!std::filesystem::is_regular_file(file))
        {
            throw InternalError{ "Could not find test file: " + file.string() };
        }
    }

    auto inContent  = std::stringstream{};
    auto outContent = std::stringstream{};

    auto inFile = std::ifstream{ inFilePath };
    if (!inFile.is_open())
    {
        throw InternalError{ "Failed to open test file: " + inFilePath.string() };
    }
    inContent << inFile.rdbuf();
    inFile.close();

    auto outFile = std::ifstream{ outFilePath };
    if (!outFile.is_open())
    {
        throw InternalError{ "Failed to open test file: " + outFilePath.string() };
    }
    outContent << outFile.rdbuf();
    outFile.close();

    return std::make_tuple(std::move(inContent).str(), std::move(outContent).str());
}

std::filesystem::path getBenchmarkPath(
    const std::filesystem::path &benchmarksFolder,
    const std::string           &executableName)
{
    if (!std::filesystem::is_directory(benchmarksFolder))
    {
        throw std::runtime_error{
            "Cannot find benchmarks folder (please create it): " +
            benchmarksFolder.string()
        };
    }

    if (auto path = benchmarksFolder / (executableName + ".json");
        !std::filesystem::exists(path))
    {
        return path;
    }

    for (int i = 1; i < 100; ++i)
    {
        auto path =
            benchmarksFolder / (executableName + "-" + std::to_string(i) + ".json");

        if (!std::filesystem::exists(path))
            return path;
    }

    throw std::runtime_error{ "Too many benchmark files for " + executableName +
                              " exists" };
}

/**
 * @brief adds additional cmd args during runtime
 */
class CmdArgs
{
public:
    CmdArgs(int argc, char **argv)
    {
        if (argv == nullptr)
            throw InternalError{ "argv cannot be nullptr" };

        _argc = argc;

        for (int i = 0; i < argc; ++i)
            _argv.push_back(argv[i]);
    }

    CmdArgs(const CmdArgs &) = delete;
    CmdArgs(CmdArgs &&)      = delete;

    CmdArgs &operator=(const CmdArgs &) = delete;
    CmdArgs &operator=(CmdArgs &&)      = delete;

    ~CmdArgs() = default;

    void add(std::string arg)
    {
        ++_argc;
        _additionalArgsHolder.push_back(std::move(arg));
        _argv.push_back(_additionalArgsHolder.back().data());
    }

    int   *argc() { return &_argc; }
    char **argv() { return _argv.data(); }

private:
    int                 _argc;
    std::vector<char *> _argv;

    std::vector<std::string> _additionalArgsHolder;
};

int main(int argc, char **argv)
{
    try
    {
        const std::string benchmarksFolder{ HW1_BENCHMARKS_FOLDER };
        const std::string executableName{ HW1_EXECUTABLE_NAME };

        const auto benchmarkPath =
            getBenchmarkPath(benchmarksFolder, executableName);

        auto args = CmdArgs{ argc, argv };

        args.add("--benchmark_out_format=json");
        args.add("--benchmark_out=" + benchmarkPath.string());

        args.add("--gtest_catch_exceptions=0");
        args.add("--gtest_break_on_failure");

        ::testing::InitGoogleTest(args.argc(), args.argv());
        const auto testsOk = RUN_ALL_TESTS();
        if (testsOk != 0)
        {
            std::cerr << "Failed to passed some tests, exiting..." << std::endl;
            return 1;
        }

        std::cout << "-----------------------" << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << "Tests passed, launching benchmarks" << std::endl;
        std::cout << "Writing benchmark to " << benchmarkPath.string() << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << "-----------------------" << std::endl;

        ::benchmark::Initialize(args.argc(), args.argv());
        ::benchmark::RunSpecifiedBenchmarks();
        ::benchmark::Shutdown();

        return 0;
    }
    catch (const InternalError &e)
    {
        std::cerr << "Internal error occured (should not happen): " << e.what()
                  << std::endl;
        throw;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception occured: " << e.what() << std::endl;
        throw;
    }
}

}    // namespace Utils

int main(int argc, char **argv)
{
    return Utils::main(argc, argv);
}
