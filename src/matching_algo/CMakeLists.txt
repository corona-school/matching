# cmake version
cmake_minimum_required(VERSION 2.8.3)

# project name
project(coronaschool-matching)

# compile options
add_compile_options(-std=c++17)


# include headers
include_directories(.)

# add all cpp-files as sources
file(GLOB SOURCES "src/*.cpp")

# executable
add_executable(matching ${SOURCES})