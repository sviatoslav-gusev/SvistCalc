cmake_minimum_required(VERSION 3.17) # required for debug/release at the same time
project(SvistCalc VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# All: lib, binary - in same dir
set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")


add_subdirectory(shared)
add_subdirectory(engine_dll)
add_subdirectory(client)