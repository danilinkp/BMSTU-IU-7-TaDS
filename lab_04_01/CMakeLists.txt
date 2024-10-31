cmake_minimum_required(VERSION 3.30)
project(lab_04_01 C)

include_directories("inc/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -g3 -Wfloat-conversion")

set(SOURCES src/main.c
        src/array_stack.c
        src/list_stack.c
        src/stack.c
        src/menu.c
        src/timer.c)

add_executable(app ${SOURCES})