cmake_minimum_required(VERSION 3.8)
project(OSWet2)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES main.cpp ATM.cpp ATM.h common.h Account.cpp Account.h Bank.cpp Bank.h PrintSafe.cpp PrintSafe.h)
add_executable(OSWet2 ${SOURCE_FILES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(OSWet2 Threads::Threads)
