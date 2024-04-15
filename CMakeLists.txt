cmake_minimum_required(VERSION 3.22)
project(warmup2 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# compiler flags
add_compile_options(-Wall -Wextra -Wpedantic)

# check memory leaks and undefined behavior in debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Debug mode")
  add_compile_definitions(NDEBUG=0)
else()
  add_compile_definitions(NDEBUG=1)
endif()

add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
add_link_options(-fsanitize=address,undefined)

add_subdirectory(src)
