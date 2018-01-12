cmake_minimum_required(VERSION 3.1)
project(jags)
set(JAGS_MAJOR 5)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
find_package(MKL REQUIRED)

include_directories("." "src/include")
add_subdirectory(src)
