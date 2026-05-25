cmake_minimum_required(VERSION 3.22)
project("pinkreader_native" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Native library source
add_library(pinkreader_native SHARED
    pinkreader_native.cpp
)

# Find required packages
find_library(log-lib log)

target_link_libraries(pinkreader_native
    ${log-lib}
)

target_include_directories(pinkreader_native PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
