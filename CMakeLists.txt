cmake_minimum_required(VERSION 3.20)

project(FractionalDE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB_RECURSE FDE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Header files
file(GLOB_RECURSE FDE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/fde/*.hpp"
)

# Library
add_library(fde STATIC
    ${FDE_SOURCES}
    ${FDE_HEADERS}
)

target_include_directories(fde PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Executable
add_executable(main main.cpp)

target_link_libraries(main PRIVATE fde)