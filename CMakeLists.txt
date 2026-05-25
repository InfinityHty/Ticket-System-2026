cmake_minimum_required(VERSION 3.28)
project(Ticket_System_2026 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Ticket_System_2026
        bpt_Database.h
        bpt_storage_test.cpp
        oj_submit.cpp)
