cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(hw1 LANGUAGES CXX)

include(FetchContent)

set(BUILD_GMOCK OFF CACHE BOOL "Builds the googlemock subproject" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF." FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "Build Release candidates with -Werror." FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Enable benchmark unit tests" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Enable benchmark unit tests which depend on gtest" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Enable installation of benchmark" FORCE)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.2
)
FetchContent_MakeAvailable(benchmark)

function(addTask source_file tests_subfolder methodName)
    file(GLOB_RECURSE utils_sources ./utils/**.h ./utils/**.cpp)

    get_filename_component(executable_name ${source_file} NAME_WLE)

    add_executable(${executable_name} ${source_file} ${utils_sources})
    set_target_properties(${executable_name} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    target_link_libraries(${executable_name} PRIVATE gtest gtest_main benchmark::benchmark)
    target_include_directories(${executable_name} PRIVATE .)

    target_compile_definitions(${executable_name} PRIVATE HW1_BENCHMARKS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/benchmarks")
    target_compile_definitions(${executable_name} PRIVATE HW1_TESTS_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/tests/${tests_subfolder}")
    target_compile_definitions(${executable_name} PRIVATE HW1_EXECUTABLE_NAME="${executable_name}")
    target_compile_definitions(${executable_name} PRIVATE HW1_METHOD_NAME=${methodName})
    target_compile_definitions(${executable_name} PRIVATE HW1_METHOD_NAME_STR="${methodName}")
endfunction()

addTask(task0-exponentiation-naive.cpp exponentiation Naive)
addTask(task0-exponentiation-binary.cpp exponentiation Binary)

addTask(task1-num-multiplication-grade-school.cpp num-multiplication GradeSchool)
addTask(task1-num-multiplication-karatsuba.cpp num-multiplication Karatsuba)

addTask(task2-matrix-multiplication-row-column.cpp matrix-multiplication RowColumn)
addTask(task2-matrix-multiplication-strassen.cpp matrix-multiplication Strassen)
addTask(task2-matrix-multiplication-row-column-with-bT.cpp matrix-multiplication RowColumnWithBT)
