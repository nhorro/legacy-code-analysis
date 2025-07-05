cmake_minimum_required(VERSION 3.10)
project(calc_jobs)

set(CMAKE_CXX_STANDARD 17)

add_executable(calc_jobs
    main.cpp
)

target_link_libraries(calc_jobs
    pthread
)