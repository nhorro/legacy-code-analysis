cmake_minimum_required(VERSION 3.10)
project(calc_jobs_instr)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

# Instrumentación de funciones
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finstrument-functions -g")

#set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

add_executable(calc_jobs_instr
    main.cpp

    instrumentation.cpp
)

target_link_libraries(calc_jobs_instr PRIVATE 
    dl 
    Threads::Threads    
)