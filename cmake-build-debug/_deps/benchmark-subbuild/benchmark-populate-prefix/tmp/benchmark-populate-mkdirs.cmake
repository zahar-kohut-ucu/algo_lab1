# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-src"
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-build"
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix"
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix/tmp"
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix/src/benchmark-populate-stamp"
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix/src"
  "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix/src/benchmark-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix/src/benchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/zahar/OneDrive/UCU/2_year/Algos/algo_lab1/cmake-build-debug/_deps/benchmark-subbuild/benchmark-populate-prefix/src/benchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
