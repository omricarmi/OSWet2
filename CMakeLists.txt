cmake_minimum_required(VERSION 3.8)
project(OSWet2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ATM.cpp ATM.h consts.h Account.cpp Account.h Bank.cpp Bank.h)
add_executable(OSWet2 ${SOURCE_FILES})