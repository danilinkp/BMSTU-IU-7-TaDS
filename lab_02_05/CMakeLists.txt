cmake_minimum_required(VERSION 3.30)
project(lab_02_05 C)

include_directories("inc/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion")

set(SRCDIR src/)
set(SOURCES ${SRCDIR}main.c ${SRCDIR}theatre.c ${SRCDIR}table_theatre.c ${SRCDIR}tools.c
        src/menu.c)

add_executable(app ${SOURCES})
