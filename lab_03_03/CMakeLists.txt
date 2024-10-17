cmake_minimum_required(VERSION 3.30)
project(lab_03_03 C)

include_directories("inc/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion")

set(SOURCES src/main.c
        src/matrix.c
        src/vector.c
        src/mv_operations.c
        src/menu.c
        src/timer.c)

add_executable(app ${SOURCES})