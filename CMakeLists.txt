cmake_minimum_required(VERSION 3.22.1)
project(Ticket-System-2026)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)
add_compile_options(-Ofast)

add_executable(code
        include/exception.h
        include/vector.h

        include/Account.h
        include/BPT_Database.h
        include/Keys.h
        include/Order.h
        include/Ticket.h
        include/TicketManagement.h
        include/Tool_functions.h
        include/Train.h
        include/TrainSystem.h
        include/User.h

        src/main.cpp
)