cmake_minimum_required(VERSION 3.17)
project(WarioEconomics)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(WarioEconomics main.cpp LList.cpp LList.h Node.cpp Node.h)

