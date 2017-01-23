cmake_minimum_required(VERSION 3.6)
project(prime_watch_dump)

set(CMAKE_CXX_STANDARD 11)

#add_compile_options(-E)

add_definitions(-DPRIME_DUMP_JSON=true)

add_subdirectory(PrimeMemoryDumping)
include_directories(PrimeMemoryDumping/common)
include_directories(PrimeMemoryDumping/)

set(SOURCE_FILES main.cpp
        GameMemory.cpp)
link_libraries(primememorydumping)
add_executable(prime_watch_dump ${SOURCE_FILES})