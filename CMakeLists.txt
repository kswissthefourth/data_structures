cmake_minimum_required(VERSION 3.21)

project(Data_Structures VERSION 1.0 LANGUAGES C)

# Add standard GCC flags
add_compile_options(-std=gnu99 -g -Wall -Wextra -pedantic)

# Set configurations
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Build executables
add_executable(linked_list ${PROJECT_SOURCE_DIR}/list_main.c ${PROJECT_SOURCE_DIR}/linked_list.c)
