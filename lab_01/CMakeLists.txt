cmake_minimum_required(VERSION 3.10)

project(BigNumProject C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion -Wvla")

add_executable(main main.c bignum.c)