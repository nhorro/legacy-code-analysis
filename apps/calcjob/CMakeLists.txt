cmake_minimum_required(VERSION 3.10)
project(calc_jobs)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

add_executable(calc_jobs
    main.cpp
)

target_link_libraries(calc_jobs PRIVATE 
    Threads::Threads    
)