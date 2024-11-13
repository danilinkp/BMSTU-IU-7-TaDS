cmake_minimum_required(VERSION 3.30)
project(lab_05_03 C)

include_directories("inc/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -g3 -Wfloat-conversion")

set(SOURCES src/main.c
        src/array_queue.c
        src/list_queue.c
        src/queue.c
        src/queue_simulation.c
        src/menu.c
        src/timer.c)

add_executable(app ${SOURCES})