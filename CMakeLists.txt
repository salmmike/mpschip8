cmake_minimum_required(VERSION 3.11)

project(mpschip8)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Wpedantic)

set(LIBNAME ${PROJECT_NAME}lib)

enable_testing()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

